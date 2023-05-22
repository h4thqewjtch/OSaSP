#define _POSIX_C_SOURCE 200112L

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define nameof(name) #name
#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

struct index_s
{
    double time_mark;
    uint64_t recno;
} **mergedData;

struct block
{
    struct index_s *offset;
    bool isTaken;
} *blocks;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

int extent = 0;

int pageIsSorted = 0;
int pageIsMerged = 0;

int comparator(const void *a, const void *b)
{
    struct index_s *first_entry = (struct index_s *)a;
    double x = (const double)first_entry->time_mark;
    struct index_s *second_entry = (struct index_s *)b;
    double y = (const double)second_entry->time_mark;
    if (x < y)
    {
        return -1;
    }
    else if (x > y)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void pthreads(void *args[])
{
    int index = atoi(args[1]);
    int blockSize = atoi(args[0]);
    int blocksAmount = getpagesize() / blockSize;
    printf("Start thread %d\n", index);
    int status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        printf("sort_thread.Barrier has been overcome\n");
    }
    else if (status != 0)
    {
        ERROR_HANDLER("pthread_barrier_wait", nameof(main))
        return;
    }
    for (int i = index; i < blocksAmount; i++)
    {
        if (!blocks[i].isTaken)
        {
            if (pthread_mutex_lock(&mutex) != 0)
            {
                ERROR_HANDLER("pthread_mutex_lock", nameof(pthreads))
                return;
            }
            pageIsSorted++;
            int entries_amount = blockSize / sizeof(struct index_s);
            struct index_s *start = blocks[i].offset;
            // int entries_amount = t_args.blocks[i].block_size / sizeof(struct index_s);
            // struct index_s *start = t_args.blocks[i].offset;
            blocks[i].isTaken = 1;
            if (pthread_mutex_unlock(&mutex) != 0)
            {
                ERROR_HANDLER("pthread_mutex_unlock", nameof(pthreads))
                return;
            }
            qsort((void *)start, entries_amount, sizeof(struct index_s), comparator);
            if (pageIsSorted >= blocksAmount)
            {
                break;
            }
        }
    }
    status = pthread_barrier_wait(&barrier);
    if (status == PTHREAD_BARRIER_SERIAL_THREAD)
    {
        printf("sort_thread.Barrier has been overcome\n");
    }
    else if (status != 0)
    {
        ERROR_HANDLER("pthread_barrier_wait", nameof(main))
        return;
    }

    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    for (int k = 0; k < extent; k++)
    {
        blocksAmount /= (1 << k);
        blockSize *= 2;
        status = pthread_barrier_wait(&barrier);
        if (status == PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("merge_thread.Barrier has been overcome\n");
        }
        else if (status != 0)
        {
            ERROR_HANDLER("pthread_barrier_wait", nameof(main))
            return;
        }
        for (int j = index * 2; j < blocksAmount; j = j + 2)
        {
            if (!blocks[j].isTaken && j % 2 == 0)
            {
                if (pthread_mutex_lock(&mutex) != 0)
                {
                    ERROR_HANDLER("pthread_mutex_lock", nameof(pthreads))
                    return;
                }

                blocks[j].isTaken = 1;
                pageIsMerged++;
                struct block firstBlock = blocks[j];
                blocks[j + 1].isTaken = 1;
                pageIsMerged++;
                struct block secondBlock = blocks[j + 1];
                int entriesAmount = blockSize / sizeof(struct index_s);
                for (int i = 0; i < entriesAmount; i++)
                {
                    mergedData[j][i].time_mark = firstBlock.offset[i].time_mark;
                    mergedData[j][i].recno = firstBlock.offset[i].recno;
                }
                for (int i = 0; i < entriesAmount; i++)
                {

                    mergedData[j][i + entriesAmount].time_mark = secondBlock.offset[i].time_mark;
                    mergedData[j][i + entriesAmount].recno = secondBlock.offset[i].recno;
                }

                if (pthread_mutex_unlock(&mutex) != 0)
                {
                    ERROR_HANDLER("pthread_mutex_unlock", nameof(pthreads))
                    return;
                }
            }
            if (pageIsMerged >= blocksAmount)
            {
                break;
            }
        }
        status = pthread_barrier_wait(&barrier);
        if (status == PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("merge_thread.Barrier has been overcome\n");
        }
        else if (status != 0)
        {
            ERROR_HANDLER("pthread_barrier_wait", nameof(main))
            return;
        }
    }
    sleep(1);
}

int main(int argc, char *argv[])
{
    int pageSize = atoi(argv[1]);
    //int pageSize = 4096;
    if (pageSize % getpagesize() || !pageSize)
    {
        printf("The pagesize is incorrect!\n");
        return -1;
    }
    int pageRecordCount = pageSize / sizeof(struct index_s);
    int threadsAmount = atoi(argv[3]);
    //int threadsAmount = 7;
    if (threadsAmount < 7 || threadsAmount > 8000)
    {
        printf("The threads amount is incorrect!\n");
        return -1;
    }

    int blocksAmount = atoi(argv[2]);
    //int blocksAmount = 8;
    if (blocksAmount <= threadsAmount || (blocksAmount & (blocksAmount - 1)))
    {
        printf("The blocks amount is incorrect!\n");
        return -1;
    }
    int blockSize = pageSize / blocksAmount;

    for (int result = blocksAmount;;)
    {
        result /= 2;
        if (result)
        {
            extent++;
        }
        else
        {
            break;
        }
    }
    printf("Extent = %d\n", extent);

    pthread_t pthread[threadsAmount];
    int fd = 0;

    if ((fd = open(argv[4], O_RDWR)) == -1)
    {
        ERROR_HANDLER("open", nameof(main))
        return -1;
    }
    int fileSize = lseek(fd, 0, SEEK_END);
    int pageCount = fileSize / pageSize;
    void **fileData = NULL;
    if (!(fileData = (void **)malloc(pageCount * sizeof(void *))))
    {
        ERROR_HANDLER("malloc", nameof(main))
        return -1;
    }
    for (int i = 0; i < pageCount; i++)
    {
        if (!(fileData[i] = (void *)malloc(sizeof(void))))
        {
            ERROR_HANDLER("malloc", nameof(main))
            return -1;
        }
    }
    int mSize = blocksAmount;
    if (!(mergedData = (struct index_s **)malloc(mSize * sizeof(struct index_s *))))
    {
        ERROR_HANDLER("malloc", nameof(main))
        return -1;
    }
    for (int i = 0; i < blockSize; i++)
    {
        if (!(mergedData[i] = (struct index_s *)malloc((pageSize/2)*sizeof(struct index_s))))
        {
            ERROR_HANDLER("malloc", nameof(main))
            return -1;
        }
    }
    char symbol = 0;
    lseek(fd, 0, SEEK_SET);
    for (int f = 0; f < pageCount; f++)
    {
        if (pthread_barrier_init(&barrier, NULL, threadsAmount) != 0)
        {
            ERROR_HANDLER("pthread_barrier_init", nameof(main))
            return -1;
        }
        int blocksAmount = atoi(argv[2]);
        int blockSize = pageSize / blocksAmount;
        fileData[f] = mmap(NULL, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, f * pageSize);
        struct index_s *data = (struct index_s *)fileData[f];
        // for (int i = 0; i < pageRecordCount; i++)
        // {
        //     printf("time mark: %lf  recno: %ld\n", data[i].time_mark, data[i].recno); ///////////////////////////////////////////////////////////////////////////////////////////////
        // }
        blocks = (struct block *)malloc(blocksAmount * sizeof(struct block));
        for (int i = 0; i < blocksAmount; i++)
        {
            blocks[i].isTaken = 0;
            blocks[i].offset = data + (i * blockSize) / sizeof(struct index_s);
        }
        for (int i = 1; i < threadsAmount; i++)
        {
            char bSize[16];
            snprintf(bSize, 16, "%d", blockSize);
            char index[16];
            snprintf(index, 16, "%d", i);
            char *args[] = {bSize, index, (char *)0};
            if (pthread_create(&pthread[i], NULL, (void *)pthreads, (void **)args) != 0)
            {
                ERROR_HANDLER("pthread_create", nameof(main))
                return -1;
            }
            sleep(1);
        }
        int status = pthread_barrier_wait(&barrier);
        if (status == PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("Barrier has been overcome\n");
        }
        else if (status != 0)
        {
            ERROR_HANDLER("pthread_barrier_wait", nameof(main))
            return -1;
        }
        for (int i = 0; i < blocksAmount; i++)
        {
            if (!blocks[i].isTaken)
            {
                if (pthread_mutex_lock(&mutex) != 0)
                {
                    ERROR_HANDLER("pthread_mutex_lock", nameof(main))
                    return -1;
                }
                pageIsSorted++;
                blocks[i].isTaken = 1;
                struct index_s *start = blocks[i].offset;
                if (pthread_mutex_unlock(&mutex) != 0)
                {
                    ERROR_HANDLER("pthread_mutex_unlock", nameof(main))
                    return -1;
                }
                qsort((void *)start, blockSize / sizeof(struct index_s), sizeof(struct index_s), comparator);
                if (pageIsSorted >= blocksAmount)
                {
                    break;
                }
            }
        }
        status = pthread_barrier_wait(&barrier);
        if (status == PTHREAD_BARRIER_SERIAL_THREAD)
        {
            printf("Barrier has been overcome\n");
        }
        else if (status != 0)
        {
            ERROR_HANDLER("pthread_barrier_wait", nameof(main))
            return -1;
        }
        pageIsSorted = 0;

        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
        // XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

        for (int k = 0; k < extent; k++)
        {
            pageIsMerged = 0;
            blocksAmount /= (1 << k);
            blockSize *= 2;
            for (int i = 0; i < blocksAmount; i++)
            {
                blocks[i].isTaken = 0;
                blocks[i].offset = data + (i * blockSize) / sizeof(struct index_s);
            }
            status = pthread_barrier_wait(&barrier);
            if (status == PTHREAD_BARRIER_SERIAL_THREAD)
            {
                printf("Barrier has been overcome\n");
            }
            else if (status != 0)
            {
                ERROR_HANDLER("pthread_barrier_wait", nameof(main))
                return -1;
            }
            for (int j = 0; j < blocksAmount; j = j + 2)
            {
                if (!blocks[j].isTaken)
                {
                    if (pthread_mutex_lock(&mutex) != 0)
                    {
                        ERROR_HANDLER("pthread_mutex_lock", nameof(main))
                        return -1;
                    }

                    blocks[j].isTaken = 1;
                    pageIsMerged++;
                    struct block firstBlock = blocks[j];
                    blocks[j + 1].isTaken = 1;
                    pageIsMerged++;
                    struct block secondBlock = blocks[j + 1];
                    int entriesAmount = blockSize / sizeof(struct index_s);
                    for (int i = 0; i < entriesAmount; i++)
                    {
                        mergedData[j][i].time_mark = firstBlock.offset[i].time_mark;
                        mergedData[j][i].recno = firstBlock.offset[i].recno;
                    }
                    for (int i = 0; i < entriesAmount; i++)
                    {
                        mergedData[j][i + entriesAmount].time_mark = secondBlock.offset[i].time_mark;
                        mergedData[j][i + entriesAmount].recno = secondBlock.offset[i].recno;
                    }

                    if (pthread_mutex_unlock(&mutex) != 0)
                    {
                        ERROR_HANDLER("pthread_mutex_unlock", nameof(main))
                        return -1;
                    }
                }
                if (pageIsMerged >= blocksAmount)
                {
                    break;
                }
            }
            status = pthread_barrier_wait(&barrier);
            if (status == PTHREAD_BARRIER_SERIAL_THREAD)
            {
                printf("Barrier has been overcome\n");
            }
            else if (status != 0)
            {
                ERROR_HANDLER("pthread_barrier_wait", nameof(main))
                return -1;
            }
        }
        if (pthread_barrier_destroy(&barrier) != 0)
        {
            ERROR_HANDLER("pthread_barrier_destroy", nameof(main))
            return -1;
        }
        printf("Barrier has been destroyed\n");
        void *threadStatus;
        for (int i = 1; i < threadsAmount; i++)
        {
            if (pthread_cancel(pthread[i]) != 0)
            {
                ERROR_HANDLER("pthread_cancel", nameof(main))
                exit(1);
            }
            if (pthread_join(pthread[i], &threadStatus) != 0)
            {
                ERROR_HANDLER("pthread_join", nameof(main))
                exit(1);
            }
            if (threadStatus == PTHREAD_CANCELED)
            {
                printf("thread %d was canceled\n", i);
            }
            else
            {
                printf("thread %d wasn't canceled\n", i);
            }
        }
        qsort((void *)mergedData[0], pageSize / sizeof(struct index_s), sizeof(struct index_s), comparator);
        for (int i = 0; i < pageRecordCount; i++)
        {
            printf("time mark: %lf  recno: %ld\n", mergedData[0][i].time_mark, mergedData[0][i].recno); ///////////////////////////////////////////////////////////////////////////////////////////////
        }
        //getchar();

        free(blocks);

        memcpy(fileData[f], mergedData[0],pageSize);
        munmap(fileData[f], pageSize);
    }
    if (close(fd) == -1)
    {
        ERROR_HANDLER("close", nameof(main))
        return -1;
    }
    free(mergedData);
    free(fileData);
    pthread_mutex_destroy(&mutex);
    exit(0);
}