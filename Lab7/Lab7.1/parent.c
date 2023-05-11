#include "message_queue.h"

pthread_t prodTids[16];
pthread_t consTids[16];

int prodMax = 0;
int consMax = 0;

bool allowProducerExit = 0;
bool allowConsumerExit = 0;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t prodCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consCond = PTHREAD_COND_INITIALIZER;

int prodCondMax = 0;
int consCondMax = 0;

struct mess;
struct Queue *messQueue;
int queueSize = 16;

int choice = 0;
void *threadStatus;

void atexit_handler();
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
    init_queue();
    messQueue->count = 0;
    while (1)
    {
        printf("  [1] :             show the list of threads;\n");
        printf("  [2] :             create a producer;\n");
        printf("  [-2]:             kill the last producer thread;\n");
        printf("  [3] :             create a consumer;\n");
        printf("  [-3]:             kill the last consumer thread;\n");
        printf("If you want to quit the program, choose another variant.\n");
        printf("Your choice:        ");
        rewind(stdin);
        scanf("%d", &choice);
        if (choice == 1)
        {
            printf("\nShow the list of threads\n\n");
            list_threads();
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
    if (pthread_cond_destroy(&prodCond) != 0)
    {
        ERROR_HANDLER("pthread_cond_destroy", nameof(prodCond));
    }
    if (pthread_cond_destroy(&consCond) != 0)
    {
        ERROR_HANDLER("pthread_cond_destroy", nameof(consCond));
    }
    if (pthread_mutex_destroy(&mut) != 0)
    {
        ERROR_HANDLER("pthread_mutex_destroy", nameof(mut));
    }
    free(messQueue);
    exit(0);
}

int create_producer()
{
    if (pthread_create(&prodTids[prodMax], NULL, (void *)producer, NULL) == -1)
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
    if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0)
    {
        ERROR_HANDLER("pthread_setcancelstate", nameof(producer));
    }
    while (1)
    {
        if (messQueue->count < 16)
        {
            if (pthread_mutex_lock(&mut) != 0)
            {
                ERROR_HANDLER("pthread_mutex_lock", nameof(mut));
            }
            while (messQueue->count >= 16)
            {
                printf("\nMessage queue is full!\n\n");
                if (pthread_cond_wait(&prodCond, &mut) != 0)
                {
                    ERROR_HANDLER("pthread_cond_wait", nameof(prodCond));
                }
            }

            push(mess_init(msg));
            printf("tid: %5d ", (int)pthread_self());
            printf("type: %c hash:%8d size: %3d data: %4d pushed:%2d ",
                   (messQueue->info[messQueue->tail - 1]).type, (messQueue->info[messQueue->tail - 1]).hash,
                   (messQueue->info[messQueue->tail - 1]).size, (messQueue->info[messQueue->tail - 1]).data, messQueue->pushed);
            printf("in line:%2d\n", messQueue->count);

            if (messQueue->count > 0)
            {
                if (pthread_cond_broadcast(&consCond) != 0)
                {
                    ERROR_HANDLER("pthread_cond_broadcast", nameof(consCond));
                }
            }

            if (pthread_mutex_unlock(&mut) != 0)
            {
                ERROR_HANDLER("pthread_mutex_unlock", nameof(mut));
            }
        }
        if (allowProducerExit)
        {
            if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0)
            {
                ERROR_HANDLER("pthread_setcancelstate", nameof(producer));
            }
        }
        sleep(1);
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
    if (pthread_create(&consTids[consMax], NULL, (void *)consumer, NULL) == -1)
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
    if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0)
    {
        ERROR_HANDLER("pthread_setcancelstate", nameof(consumer));
    }
    while (1)
    {
        if (messQueue->count)
        {
            if (pthread_mutex_lock(&mut) != 0)
            {
                ERROR_HANDLER("pthread_mutex_lock", nameof(mut));
            }
            while (messQueue->count <= 0)
            {
                printf("\nMessage queue is empty!\n\n");
                if (pthread_cond_wait(&consCond, &mut) != 0)
                {
                    ERROR_HANDLER("pthread_cond_wait", nameof(consCond));
                }
            }

            var = pop();
            printf("tid: %5d ", (int)pthread_self());
            printf("type: %c hash:%8d size: %3d data: %4d poped:%3d ", var.type, var.hash, var.size, var.data, messQueue->poped);
            printf("in line:%2d\n", messQueue->count);

            if (messQueue->count < queueSize)
            {
                if (pthread_cond_broadcast(&prodCond) != 0)
                {
                    ERROR_HANDLER("pthread_cond_signal", nameof(prodCond));
                }
            }

            if (pthread_mutex_unlock(&mut) != 0)
            {
                ERROR_HANDLER("pthread_mutex_unlock", nameof(mut));
            }
        }
        if (allowConsumerExit)
        {

            if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) != 0)
            {
                ERROR_HANDLER("pthread_setcancelstate", nameof(consumer));
            }
        }
        sleep(1);
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
        allowProducerExit = true;
        printf("\nKill the producer thread\n");
        if (pthread_cancel(prodTids[prodMax - 1]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(prodTids[prodMax - 1]));
            exit(1);
        }
        if (pthread_join(prodTids[prodMax - 1], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(prodTids[prodMax - 1]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Producer №%d was canceled\n", prodMax - 1);
            prodMax--;
        }
        else
        {
            printf("Producer №%d wasn't canceled\n", prodMax - 1);
        }
        allowProducerExit = false;
    }
    else if (choice == -3 && consMax > 0)
    {
        allowConsumerExit = true;
        printf("\nKill the consumer thread\n");
        if (pthread_cancel(consTids[consMax - 1]) == -1)
        {
            ERROR_HANDLER("pthread_cancel", nameof(consTids[consMax - 1]));
            exit(1);
        }
        int status = 0;
        if (pthread_join(consTids[consMax - 1], &threadStatus) == -1)
        {
            ERROR_HANDLER("pthread_join", nameof(consTids[consMax - 1]));
            exit(1);
        }
        if (threadStatus == PTHREAD_CANCELED)
        {
            printf("Consumer №%d was canceled\n", consMax - 1);
            consMax--;
        }
        else
        {
            printf("Consumer №%d wasn't canceled\n", consMax - 1);
        }
        allowConsumerExit = false;
    }
    else
    {
        printf("Thread wasn't canceled!\n");
    }
}

void atexit_handler()
{
    allowProducerExit = true;
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
    allowConsumerExit = true;
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
}