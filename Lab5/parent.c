#include "message_queue.h"

pthread_t prodTids[16];
pthread_t consTids[16];

int prodMax = 0;
int consMax = 0;

struct mess;
struct Queue *messQueue;

int choice = 0;
void *threadStatus;

int sems_init();
void atexit_handler();
void exit_unlink(const char *);
int create_producer();
void producer();
struct mess mess_init(struct mess);
int create_consumer();
void consumer();
void list_threads();
void kill_thread(int);

int main()
{
    messQueue = (struct Queue *)malloc(sizeof(struct Queue));
    sems_init();
    while (1)
    {   
        printf("  [1] :             create a message queue;\n");
        printf("  [11]:             increase the message queue;\n");
        printf("  [10]:             decrease the message queue;\n");
        printf("  [2] :             create a producer;\n");
        printf("  [-2]:             kill the last producer thread;\n");
        printf("  [3] :             create a consumer;\n");
        printf("  [-3]:             kill the last consumer thread;\n");
        printf("  [4] :             show the list of threads;\n");
        printf("If you want to quit the program, choose another variant.\n");
        printf("Your choice:        ");
        rewind(stdin);
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("\nCreate a message queue\n\n");
            init_queue();
            messQueue->count = 0;
        }
        else if (choice == 11)
        {
            printf("\nIncrease the message queue\n");
            printf("Enter a value to increase the message queue: ");
            int value = 0;
            rewind(stdin);
            scanf("%d", &value);
            printf("\n");
            increase_queue(value);
        }
        else if (choice == 10)
        {
            printf("\nDecrease the message queue\n");
            printf("Enter a value to decrease the message queue: ");
            int value = 0;
            rewind(stdin);
            scanf("%d", &value);
            printf("\n");
            decrease_queue(value);
        }
        else if (choice == 2)
        {
            printf("\nCreate a producer\n\n");
            create_producer();
        }
        else if (choice == 3)
        {
            printf("\nCreate a consumer\n\n");
            create_consumer();
        }
        else if (choice == 4)
        {
            printf("\nShow the list of threads\n\n");
            list_threads();
        }
        else if (choice == -2 || choice == -3)
        {
            kill_thread(choice);
        }
        else if (choice == 0)
        {
            printf("\nQuit\n\n");
            break;
        }
        choice = 0;
    }
    atexit_handler();
    free(messQueue->info);
    free(messQueue);
    exit(0);
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

    pushSem = sem_open("pushSem",
                       (O_RDWR | O_CREAT | O_TRUNC),
                       (S_IRUSR | S_IWUSR), 256);

    if (pushSem == SEM_FAILED)
    {
        ERROR_HANDLER("sem_open", nameof(pushSem));
        return -1;
    }

    popSem = sem_open(POPSEM,
                      (O_RDWR | O_CREAT | O_TRUNC),
                      (S_IRUSR | S_IWUSR), 0);

    if (popSem == SEM_FAILED)
    {
        ERROR_HANDLER("sem_open", nameof(popSem));
        return -1;
    }
    return 0;
}

int create_producer()
{
    if (pthread_create(&prodTids[prodMax], NULL, (void *)producer, messQueue) == -1)
    {
        ERROR_HANDLER("pthread_create", nameof(prodTids[prodMax]));
        return -1;
    }
    prodMax++;
}

void producer()
{
    printf("Start producer\n");
    struct mess msg;
    while (1)
    {
        sem_wait(pushSem);
        sem_wait(mutex);

        push(mess_init(msg));
        printf("tid: %5d ", (int)pthread_self());
        printf("type: %c hash:%8d size: %3d data: %4d pushed:%2d ",
               (messQueue->info[messQueue->tail - 1]).type, (messQueue->info[messQueue->tail - 1]).hash,
               (messQueue->info[messQueue->tail - 1]).size, (messQueue->info[messQueue->tail - 1]).data, messQueue->pushed);
        printf("in line:%2d\n", messQueue->count);

        sem_post(mutex);
        sem_post(popSem);

        if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0)
        {
            ERROR_HANDLER("pthread_setcancelstate", nameof(producer));
        }
        sleep(5);
    }
}

struct mess mess_init(struct mess msg)
{
    srand(time(NULL));
    msg.type = 'p';
    msg.size = rand() % 257;
    while (!msg.size)
    {
        msg.size = rand() % 257;
    }
    if (msg.size == 256)
    {
        msg.size = 0;
    }
    msg.data = rand() % 2048;
    msg.hash = 0;
    for (int i = 0, j = msg.data; i <= msg.size; i++, j--)
    {
        msg.hash += i + j;
    }
    return msg;
}

int create_consumer()
{
    if (pthread_create(&consTids[consMax], NULL, (void *)consumer, messQueue) == -1)
    {
        ERROR_HANDLER("pthread_create", nameof(consTids[consMax]));
        return -1;
    }
    consMax++;
}

void consumer()
{
    printf("Start consumer\n");
    int s;
    struct mess var = {0, 0, 0, 0};
    while (1)
    {
        sem_wait(popSem);
        sem_wait(mutex);

        var = pop();
        printf("tid: %5d ", (int)pthread_self());
        printf("type: %c hash:%8d size: %3d data: %4d poped:%3d ", var.type, var.hash, var.size, var.data, messQueue->poped);
        printf("in line:%2d\n", messQueue->count);

        sem_post(mutex);
        sem_post(pushSem);

        if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0)
        {
            ERROR_HANDLER("pthread_setcancelstate", nameof(consumer));
        }
        sleep(5);
    }
}

void list_threads()
{
    printf("The list of threads:\n");
    printf("    Parent process pid:            %d\n", getppid());
    printf("\n");
    for (int i = 0; i < prodMax; i++)
    {
        printf("    Producer thread №%2d tid:      %d\n", i, (int)prodTids[i]);
    }
    printf("\n");
    for (int i = 0; i < consMax; i++)
    {
        printf("    Consumer thread №%2d tid:      %d\n", i, (int)consTids[i]);
    }
}

void kill_thread(int choice)
{
    if (choice == -2 && prodMax > 0)
    {
        printf("\nKill the last producer thread\n");
        if (pthread_cancel(prodTids[--prodMax]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(prodTids[prodMax]));
            exit(1);
        }
        if (pthread_join(prodTids[prodMax], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(prodTids[prodMax]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Producer №%d was canceled\n", prodMax);
        }
        else
        {
            printf("Producer №%d wasn't canceled\n", prodMax);
        }
    }
    else if (choice == -3 && consMax > 0)
    {
        printf("\nKill the last consumer thread\n");
        if (pthread_cancel(consTids[--consMax]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(consTids[consMax]));
            exit(1);
        }
        int status = 0;
        if (pthread_join(consTids[consMax], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(consTids[consMax]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Consumer №%d was canceled\n", consMax);
        }
        else
        {
            printf("Consumer №%d wasn't canceled\n", consMax);
        }
    }
    else
    {
        printf("Thread wasn't canceled!\n");
    }
}

void atexit_handler()
{
    for (int i = 0; i < prodMax; i++)
    {
        if (pthread_cancel(prodTids[i]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(prodTids[i]));
            exit(1);
        }
        if (pthread_join(prodTids[i], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(prodTids[i]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Producer №%d was canceled\n", i);
        }
        else
        {
            printf("Producer №%d wasn't canceled\n", i);
        }
    }
    for (int i = 0; i < consMax; i++)
    {
        if (pthread_cancel(consTids[i]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(consTids[i]));
            exit(1);
        }
        int status = 0;
        if (pthread_join(consTids[i], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(consTids[i]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Consumer №%d was canceled\n", i);
        }
        else
        {
            printf("Consumer №%d wasn't canceled\n", i);
        }
    }
    exit_unlink(MUTEX);

    exit_unlink(POPSEM);

    exit_unlink(PUSHSEM);
}

void exit_unlink(const char *env) // удаление семафоров
{
    if (sem_unlink(env))
    {
        ERROR_HANDLER("sem_unlink", env);
        exit(1);
    }
}
