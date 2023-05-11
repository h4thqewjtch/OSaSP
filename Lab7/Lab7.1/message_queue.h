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
    int count;
    struct mess info[16];
    int head;
    int tail;

} *messQueue;

extern bool allowProducerExit;
extern bool allowConsumerExit;

void init_queue()
{
    messQueue->tail = messQueue->head = 0;
    messQueue->poped = messQueue->pushed = 0;
}

void push(struct mess msg)
{

    if (messQueue->pushed == 16)
    {
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
    msg.type = 'c';
    if (messQueue->poped == 16)
    {
        init_queue();
    }
    messQueue->poped++;
    messQueue->count--;
    return msg;
}