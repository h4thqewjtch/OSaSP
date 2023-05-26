#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdint.h>
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

struct index_s
{
    double time_mark;
    uint64_t recno;
} *rec;

int fd = 0;

FILE *file;

double get_day()
{
    return (double)(rand() % (365 * 123)) + 15020.0;
}

int main(int argc, char **argv)
{
    int size = 1;
    char *fileName = "/home/h4thqewjtch/Libs/Lab6/file.txt";
    // int size = atoi(argv[2]);
    uint64_t rec_amount = (size * (1 << 12)) / sizeof(struct index_s); // 64kb
    rec = (struct index_s *)malloc(rec_amount * sizeof(struct index_s));
    // if ((file = fopen(argv[1], "wb")) == NULL)
    if ((file = fopen(fileName, "wb")) == NULL)
    {
        printf("File wasn't open\n");
        return -1;
    }
    fclose(file);
    if ((fd = open(fileName, O_RDWR)) == -1)
    //if ((fd = open(argv[1], O_RDWR)) == -1)
    {
        ERROR_HANDLER("open", nameof(main));
        return -1;
    }
    lseek(fd, 0, SEEK_SET);

    for (uint64_t i = 0; i < rec_amount; i++)
    {
        double wholePart = (double)(rand() % (365 * 123)) + 15020.0;
        double fraction = 0.5 + rand() % 12;
        rec[i].time_mark = wholePart + fraction * 0.01;
        rec[i].recno = i;
        if (write(fd, &rec[i], sizeof(rec[i])) == -1)
        {
            ERROR_HANDLER("write", nameof(main));
            return -1;
        }
    }
    char symbol = 0;
    lseek(fd, 0, SEEK_SET);
    struct index_s r;
    while (1)
    {
        if (!read(fd, &symbol, sizeof(char)))
        {
            break;
        }
        else
        {
            lseek(fd, -1, SEEK_CUR);
        }
        if (read(fd, &r, sizeof(r)) == -1)
        {
            ERROR_HANDLER("write", nameof(main));
            return -1;
        }
        printf("time_mark: %lf index: %ld\n", r.time_mark, r.recno);
    }
    close(fd);
    return 0;
}

// for (uint64_t i = 0; i < rec_amount; i++)
// {
//     header->idx[i].recno = i;
//     header->idx[i].time_mark = get_day() + get_day_time();
// }
// int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0777);
// write(fd, header, header_size);
// printf("header size %d\n", header_size);
// close(fd);
// return 0;
// }
