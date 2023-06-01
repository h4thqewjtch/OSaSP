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

char rec[1024];

int fd = 0;

FILE *file;

int main(int argc, char **argv)
{
    char *fileName = "/home/h4thqewjtch/Libs/Lab8/file.txt";
    // if ((file = fopen(argv[1], "wb")) == NULL)
    if ((file = fopen(fileName, "wb")) == NULL)
    {
        printf("File wasn't open\n");
        return -1;
    }
    fclose(file);
    if ((fd = open(fileName, O_RDWR)) == -1)
    // if ((fd = open(argv[1], O_RDWR)) == -1)
    {
        ERROR_HANDLER("open", nameof(main));
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    strcpy(rec, "ECHO hello world\n");
    if (write(fd, rec, sizeof(rec)) == -1)
    {
        ERROR_HANDLER("write", nameof(main));
        return -1;
    }

    char symbol = 0;
    lseek(fd, 0, SEEK_SET);
    char r[1024];
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
        printf("%s", r);
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
