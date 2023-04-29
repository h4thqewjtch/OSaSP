#include "message_queue.h"

struct mess;
struct Queue *mess_queue;
bool allow_exit = 0;

pid_t producer_pids[128];
pid_t consumer_pids[128];
int prod_max = 0;
int cons_max = 0;

int choice = 0;

int sems_init();
int shar_mem_init();
void init_sig();
void atexit_handler();
void exit_unlink(const char *);
void exec_choice(int);
int create_producer();
void producer();
struct mess mess_init(struct mess);
int create_consumer();
void consumer();
void list_processes();
void send_signal(pid_t, int);

int main()
{
    sems_init();
    shar_mem_init();
    init_sig();
    while (1)
    {
        printf("  [1]:            create a message queue;\n");
        printf("  [2]:            create a producer;\n");
        printf(" [-2]:            kill the last producer process;\n");
        printf("  [3]:            create a consumer;\n");
        printf(" [-3]:            kill the last consumer process;\n");
        printf("  [4]:            show the list of processes;\n");
        printf("If you want to quit the program, choose another variant.\n");
        printf("Your choice:        ");
        rewind(stdin);
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("Create a message queue\n");
            init_queue();
            mess_queue->all_amount = 0;
        }
        else if (choice == 2)
        {
            printf("Create a producer\n");
            create_producer();
        }
        else if (choice == 3)
        {
            printf("Create a consumer\n");
            create_consumer();
        }
        else if (choice == 4)
        {
            for (int i = 0; i < prod_max; i++)
                send_signal(producer_pids[i], SIGSTOP);
            for (int i = 0; i < cons_max; i++)
                send_signal(consumer_pids[i], SIGSTOP);
            printf("The list of processes:\n");
            list_processes();
            for (int i = 0; i < prod_max; i++)
                send_signal(producer_pids[i], SIGCONT);
            for (int i = 0; i < cons_max; i++)
                send_signal(consumer_pids[i], SIGCONT);
        }
        else if (choice == -2)
        {
            prod_max--;
            printf("Kill the producer process №%d\n", prod_max);
            send_signal(producer_pids[prod_max], SIGTERM);
        }
        else if (choice == -3)
        {
            cons_max--;
            printf("Kill the consumer process №%d\n", cons_max);
            send_signal(consumer_pids[cons_max], SIGTERM);
        }
        else if (choice == 0)
        {
            printf("\nQuit\n\n");
            break;
        }
        choice = 0;
    }
    atexit_handler();
    exit(0);
}

void init_sig()
{
    struct sigaction term_action;
    memset(&term_action, 0, sizeof(term_action));
    term_action.sa_handler = &term_handler;
    sigaction(SIGKILL, &term_action, NULL);
}

int sems_init() // инициализация семафоров
{
    mutex = sem_open(MUTEX,
                     (O_RDWR | O_CREAT | O_TRUNC),
                     (S_IRUSR | S_IWUSR), 1);

    if (mutex == SEM_FAILED)
    {
        ERROR_HANDLER("sem_open", nameof(mutex));
        return -1;
    }

    push_sem = sem_open("push_sem",
                        (O_RDWR | O_CREAT | O_TRUNC),
                        (S_IRUSR | S_IWUSR), 16);

    if (push_sem == SEM_FAILED)
    {
        ERROR_HANDLER("sem_open", nameof(push_sem));
        return -1;
    }

    pop_sem = sem_open(POP_SEM,
                       (O_RDWR | O_CREAT | O_TRUNC),
                       (S_IRUSR | S_IWUSR), 0);

    if (pop_sem == SEM_FAILED)
    {
        ERROR_HANDLER("sem_open", nameof(pop_sem));
        return -1;
    }

    return 0;
}

int shar_mem_init() // инициализация общей памяти
{
    int fd = shm_open(SHM_NAME,
                      (O_CREAT | O_RDWR | O_TRUNC),
                      (S_IRUSR | S_IWUSR));
    if (fd < 0)
    {
        ERROR_HANDLER("shm_open", SHM_NAME);
        return -1;
    }

    if (ftruncate(fd, sizeof(struct Queue)) == -1)
    {
        ERROR_HANDLER("ftruncate", nameof(mess_queue));
        return -1;
    }

    void *address = mmap(NULL, sizeof(struct Queue),
                         (PROT_READ | PROT_WRITE),
                         MAP_SHARED, fd, 0);

    if (address == MAP_FAILED)
    {
        ERROR_HANDLER("mmap", nameof(address));
        return -1;
    }

    mess_queue = (struct Queue *)address;

    close(fd);

    return 0;
}

void exec_choice(int var)
{
}

int create_producer()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        ERROR_HANDLER("fork", nameof(pid));
        return -1;
    }
    if (pid == 0)
    {
        producer();
    }
    else if (pid > 0)
    {
        producer_pids[prod_max] = pid;
        prod_max++;
    }
}

void producer()
{
    allow_exit = 0;
    signal(SIGTERM, term_handler);
    printf("Start producer\n");
    struct mess msg;
    while (1)
    {
        sem_wait(push_sem);
        sem_wait(mutex);

        push(mess_init(msg));
        printf("pid: %5d  ", getpid());
        printf("type: %c hash: %8d size: %3d data: %4d pushed: %2d   ",
               (mess_queue->info[mess_queue->tail - 1]).type, (mess_queue->info[mess_queue->tail - 1]).hash,
               (mess_queue->info[mess_queue->tail - 1]).size, (mess_queue->info[mess_queue->tail - 1]).data, mess_queue->pushed);
        printf("in line: %2d\n", mess_queue->all_amount);
        sem_post(mutex);
        sem_post(pop_sem);

        if (allow_exit)
        {
            printf("allow exit producer - %d.\n\n", getpid());
            exit(1);
        }
        sleep(5);
    }
    exit(0);
}

struct mess mess_init(struct mess msg)
{
    srand(time(NULL));
    msg.type = 'p';
    msg.size = rand() % 257;
    while (!msg.size)
        msg.size = rand() % 257;
    if (msg.size == 256)
        msg.size = 0;
    msg.data = rand() % 2048;
    msg.hash = 0;
    for (int i = 0, j = msg.data; i <= msg.size; i++, j--)
        msg.hash += i + j;
    return msg;
}

int create_consumer()
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("Error occured. Error code - %d\n", errno);
        exit(errno);
    }
    if (pid == 0)
    {
        consumer();
    }
    else if (pid > 0)
    {
        consumer_pids[cons_max] = pid;
        cons_max++;
    }
}

void consumer()
{
    signal(SIGTERM, term_handler);
    printf("Start consumer\n");
    struct mess var = {0, 0, 0, 0};
    while (1)
    {
        sem_wait(pop_sem);
        sem_wait(mutex);

        var = pop();
        printf("pid: %5d  ", getpid());
        printf("type: %c hash: %8d size: %3d data: %4d poped: %3d   ", var.type, var.hash, var.size, var.data, mess_queue->poped);
        printf("in line: %2d\n", mess_queue->all_amount);
        sem_post(mutex);
        sem_post(push_sem);

        if (allow_exit)
        {
            printf("allow exit consumer - %d.\n\n", getpid());
            exit(1);
        }
        sleep(5);
    }
    exit(0);
}

void list_processes()
{
    printf("    Parent process pid:            %d\n", getppid());
    printf("\n");
    for (int i = 0; i < prod_max; i++)
        printf("    Producer process №%2d pid:      %d\n", i, producer_pids[i]);
    printf("\n");
    for (int i = 0; i < cons_max; i++)
        printf("    Consumer process №%2d pid:      %d\n", i, consumer_pids[i]);
}

void send_signal(pid_t pid, int SIGNUM)
{
    if (kill(pid, SIGNUM) == -1)
    {
        printf("Error code: %d\n", errno);
        exit(errno);
    }
}

void atexit_handler()
{
    for (int i = 0; i < prod_max; i++)
        send_signal(producer_pids[i], SIGTERM);
    for (int i = 0; i < cons_max; i++)
        send_signal(consumer_pids[i], SIGTERM);

    sleep(1);
    if (shm_unlink(SHM_NAME))
    {
        ERROR_HANDLER("shm_unlink", SHM_NAME);
    }

    exit_unlink(MUTEX);

    exit_unlink(POP_SEM);

    exit_unlink(PUSH_SEM);
}

void exit_unlink(const char *env) // удаление семафоров
{
    if (sem_unlink(env))
    {
        ERROR_HANDLER("sem_unlink", env);
    }
}
