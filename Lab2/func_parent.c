#include "func_parent.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int processNumber = 0;

void printEnv(char *envp[])
{
    int size = 0;
    while (envp[size] != NULL)
        size++;
    qsort((void *)envp, size, sizeof(char *), compare);
    for (int i = 0; envp[i] != NULL; i++)
        puts(envp[i]);
}

void createCHILD(char ch, char *argv[], char *envp[])
{
    if (ch == 'q')
        exit(0);
    int childStatus;
    processNumber++;
    __pid_t pid = fork();
    if (pid == -1)
    {
        processNumber--;
        printf("Error occured. Error code - %d", errno);
        exit(errno);
    }
    if (pid == 0)
    {
        system("clear");
        puts("Child process created. Please wait...");
        char number[16];
        sprintf(number, "%d", processNumber);
        char path[512];
        switch (ch)
        {
        case '+':
        {
            snprintf(path, sizeof(path), "%s/%s", getenv("CHILD_PATH"), argv[1]);
            break;
        }
        case '*':
        {
            snprintf(path, sizeof(path), "%s/%s", cut(envp[0]), argv[1]);
            break;
        }
        case '&':
        {
            snprintf(path, sizeof(path), "%s/%s", cut(__environ[0]), argv[1]);
            break;
        }
        }
        char *args[] = {path, number, argv[2], (char *)0};
        if (execve("./child", args, envp) == -1)
        {
            processNumber--;
            printf("Error occured. Error code - %d\n", errno);
            exit(errno);
        }
        wait(&childStatus);
        printf("Child process have been endeed with %d exit status\n", childStatus);
    }
}

int compare(const void *arg1, const void *arg2)
{
    return strcmp(*(char **)arg1, *(char **)arg2);
}

char *cut(char *str)
{
    char *pos;
    pos = strstr(str, "/");
    int p = pos - str;
    for (int i = 0; i < (int)strlen(str); i++)
        str[i] = str[p + i];
    return str;
}