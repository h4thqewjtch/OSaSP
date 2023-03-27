#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

int compare(const void *, const void *);
char *Cut(char *);

int main(int argc, char *argv[], char *envp[])
{
    int size = 0;
    while (envp[size] != NULL)
        size++;
    qsort((void *)envp, size, sizeof(char *), compare);
    for (int i = 0; envp[i] != NULL; i++)
        puts(envp[i]);
    for (int processNumber = 0; processNumber < 100;)
    {
        int childStatus;
        puts("  +:      search by getenv();");
        puts("  *:      search by *envp[];");
        puts("  &:      search by **environ;");
        puts("  q:      exit of parent process.");
        printf("Your choice:    ");
        rewind(stdin);
        char c = getchar();
        getchar();
        switch (c)
        {
        case '+':
        {
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
                puts("Child process created. Please wait...");
                char number[16];
                sprintf(number, "%d", processNumber);
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", getenv("CHILD_PATH"), argv[1]);
                char *args[] = {path, number, argv[2], (char *)0};
                if (execve("./child", args, envp) == -1)
                {
                    processNumber--;
                    printf("Error occured. Error code - %d\n", errno);
                    exit(errno);
                }
            }
            wait(&childStatus);
            printf("Child process have been endeed with %d exit status\n", childStatus);
            break;
        }
        case '*':
        {
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
                puts("Child process created. Please wait...");
                char number[16];
                sprintf(number, "%d", processNumber);
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", Cut(envp[0]), argv[1]);
                char *args[] = {path, number, argv[2], (char *)0};
                if (execve("./child", args, envp) == -1)
                {
                    processNumber--;
                    printf("Error occured. Error code - %d", errno);
                    exit(errno);
                }
            }
            wait(&childStatus);
            printf("Child process have been endeed with %d exit status\n", childStatus);
            break;
        }
        case '&':
        {
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
                puts("Child process created. Please wait...");
                extern char **__environ;
                char number[16];
                sprintf(number, "%d", processNumber);
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", Cut(__environ[0]), argv[1]);
                char *args[] = {path, number, argv[2], (char *)0};
                if (execve("./child", args, envp) == -1)
                {
                    processNumber--;
                    printf("Error occured. Error code - %d", errno);
                    exit(errno);
                }
            }
            wait(&childStatus);
            printf("Child process have been endeed with %d exit status\n", childStatus);
            break;
        }
        case 'q':
        {
            exit(0);
            break;
        }
        default:
        {
            puts("Unknown option!\n");
            break;
        }
        }
        if (c == 'q')
            break;
    }
    exit(0);
}

int compare(const void *arg1, const void *arg2)
{
    return strcmp(*(char **)arg1, *(char **)arg2);
}

char *Cut(char *str)
{
    char *pos;
    pos = strstr(str, "/");
    int p = pos - str;
    for (int i = 0; i < (int)strlen(str); i++)
        str[i] = str[p + i];
    return str;
}