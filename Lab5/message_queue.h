#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define nameof(name) #name
#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

sem_t *mutex;
sem_t *pushSem;
sem_t *popSem;

#define MUTEX "mutex"
#define PUSHSEM "pushSem"
#define POPSEM "popSem"

extern struct mess
{
    char type;
    int hash;
    unsigned int size;
    unsigned int data;
};

int queueSize = 16;

extern struct Queue
{
    int pushed;
    int poped;
    int count;
    struct mess *info;
    int head;
    int tail;

} *messQueue;

void init_queue()
{
    messQueue->tail = messQueue->head = 0;
    messQueue->poped = messQueue->pushed = 0;
    messQueue->info = (struct mess *)malloc(queueSize * sizeof(struct mess));
}

void push(struct mess msg)
{
    if (messQueue->count >= queueSize)
    {
        printf("\nMessage queue is full!\n\n");
        return;
    }

    if (messQueue->pushed == queueSize)
    {
        free(messQueue->info);
        init_queue();
    }

    messQueue->pushed++;
    messQueue->count++;
    messQueue->info[messQueue->tail] = msg;
    messQueue->tail++;
}

struct mess pop()
{
    struct mess msg = messQueue->info[messQueue->head++];
    if (messQueue->count <= 0)
    {
        printf("\nMessage queue is empty!\n\n");
        return msg;
    }
    if (messQueue->poped == queueSize)
    {
        free(messQueue->info);
        init_queue();
    }
    messQueue->poped++;
    messQueue->count--;
    msg.type = 'c';
    return msg;
}

void increase_queue(int value)
{
    int result = queueSize + value;
    if (result <= 256 && result > 0)
    {
        queueSize = result;
        if (!(messQueue->info = (struct mess *)realloc(messQueue->info, queueSize * sizeof(struct mess))))
        {
            ERROR_HANDLER("realloc", nameof(messQueue->info));
        }
    }
    else
    {
        printf("\nQueue increasing wasn't performed\n\n");
    }
}

void decrease_queue(int value)
{
    int result = queueSize - value;
    if (result >= messQueue->count && result > 0 && result <= 256)
    {
        queueSize = result;
        if (!(messQueue->info = (struct mess *)realloc(messQueue->info, queueSize * sizeof(struct mess))))
        {
            ERROR_HANDLER("realloc", nameof(messQueue->info));
        }
    }
    else
    {
        printf("\nQueue decreasing wasn't performed\n\n");
    }
}