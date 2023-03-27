#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

int compare(const void *, const void *);
char *Cut(char *);

int main(int argc, char *argv[], char *envp[])
{
    char *var = NULL;
    if (!(var = (char *)malloc(256 * sizeof(char))))
    {
        puts("Error: Memory allocation is failed!");
        return 0;
    }
    while (1)
    {
        int childStatus;
        puts("  +:            create a child process;");
        puts("  -:            delete the last child process;");
        puts("  l:            show the list of processes;");
        puts("  k:            delete all child processes;");
        puts("  s:            disable all statistics;");
        puts("  g:            able all statistics;");
        puts("  s<num>:       disable statistic for child process C_<num>;");
        puts("  g<num>:       able statistic for child process C_<num>;");
        puts("  p<num>:       able statistic for child process C_<num>;");
        puts("  q:            exit of parent process.");
        printf("Your choice: ");
        rewind(stdin);
        scanf("%s", var);
        puts(var);
        if (!strcmp(var, "+"))
        {
            system("clear");
            puts("Create a child process");
            __pid_t pid = fork();
            if (pid == -1)
            {
                printf("Error occured. Error code - %d", errno);
                exit(errno);
            }
            if (pid == 0)
            {
                printf("Child process created. Please wait...\n");
                char *args[] = {argv[1], argv[2], (char *)0};
                execve(argv[1], args, (void *)0);
            }
            wait(&childStatus);
            printf("Child process have been endeed with %d exit status\n", childStatus);
        }
        else if (!strcmp(var, "-"))
        {
            system("clear");
            puts("Delete the last child process");
        }
        else if (!strcmp(var, "l"))
        {
            system("clear");
            puts("Show the list of processes");
        }
        else if (!strcmp(var, "k"))
        {
            system("clear");
            puts("Delete all child processes");
        }
        else if (strstr(var, "s"))
        {

            if (strlen(var) == 1)
            {
                system("clear");
                puts("Disable all statistics");
            }
            else if (var[1] >= 48 && var[1] <= 57)
            {
                system("clear");
                int number = 0;
                int i = 0;
                for (int i = 0; var[i + 1] != '\0'; i++)
                {
                    int p =(int)pow(10.0, i);
                    p *= (var[i + 1] - 48);
                    number += p;
                }
                printf("Disable statistic for child process C_%d", number);
            }
            else
                puts("Unknown option!");
        }
        else if (strstr(var, "g"))
        {
            if (strlen(var) == 1)
            {
                system("clear");
                puts("Create a child process");
            }
            else if (var[1] >= 48 && var[1] <= 57)
            {
                system("clear");
                int number = 0;
                int i = 0;
                for (int i = 0; var[i + 1] != '\0'; i++)
                {
                    int p = pow(10, i);
                    p *= (var[i + 1] - 48);
                    number += p;
                }
                printf("Able statistic for child process C_%d", number);
            }
            else
                puts("Unknown option!");
        }
        else if (strstr(var, "p"))
        {
            if (var[1] >= 48 && var[1] <= 57)
            {
                system("clear");
                int number = 0;
                int i = 0;
                for (int i = 0; var[i + 1] != '\0'; i++)
                {
                    int p = pow(10, i);
                    p *= (var[i + 1] - 48);
                    number += p;
                }
                printf("Able statistic for child process C_%d", number);
            }
            else
                puts("Unknown option!");
        }
        else if (!strcmp(var, "q"))
            break;
        else
            puts("Unknown option!");
    }
    free(var);
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