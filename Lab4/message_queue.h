#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SA_RESTART 0x10000000

#define nameof(name) #name
#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

sem_t *mutex;
sem_t *push_sem;
sem_t *pop_sem;

#define SHM_NAME "/shmem"
#define MUTEX "mutex"
#define PUSH_SEM "push_sem"
#define POP_SEM "pop_sem"

extern struct mess
{
    char type;
    int hash;
    unsigned int size;
    unsigned int data;
};

extern struct Queue
{
    int pushed;
    int poped;
    int all_amount;
    struct mess info[16];
    int head;
    int tail;
} *mess_queue;

extern bool allow_exit;

void init_queue()
{
    mess_queue->tail = mess_queue->head = 0;
    mess_queue->poped = mess_queue->pushed = 0;
}

void push(struct mess msg)
{
    if (mess_queue->all_amount > 16)
    {
        ERROR_HANDLER("push", nameof(mess_queue->all_amount));
        exit(1);
    }

    if (mess_queue->pushed == 16)
    {
        init_queue();
    }


    mess_queue->pushed++;
    mess_queue->all_amount++;
    mess_queue->info[mess_queue->tail] = msg;
    mess_queue->tail++;
}

struct mess pop()
{
    if (mess_queue->all_amount < 0)
    {
        ERROR_HANDLER("pop", nameof(mess_queue->all_amount));
        exit(1);
    }
    struct mess msg = mess_queue->info[mess_queue->head++];
    mess_queue->poped++;
    mess_queue->all_amount--;
    msg.type = 'c';
    return msg;
}

void term_handler() // обработчик сигнала сигтерм
{
    allow_exit = 1;
}