#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

void printEnv(char **);
void menu(char **, char **);
void createCHILD(char, char **, char **);
int compare(const void *, const void *);
char *cut(char *);

int processNumber = 0;

int main(int argc, char *argv[], char *envp[])
{
    printEnv(envp);
    menu(argv, envp);
    exit(0);
}

void printEnv(char *envp[])
{
    int size = 0;
    while (envp[size] != NULL)
        size++;
    qsort((void *)envp, size, sizeof(char *), compare);
    for (int i = 0; envp[i] != NULL; i++)
        puts(envp[i]);
}

void menu(char *argv[], char *envp[])
{
    while (processNumber < 100)
    {
        puts("  +:      search by getenv();");
        puts("  *:      search by *envp[];");
        puts("  &:      search by **environ;");
        puts("  q:      exit of parent process.");
        printf("Your choice:    ");
        char ch = 0;
        rewind(stdin);
        scanf("%c", &ch);
        getchar();
        createCHILD(ch, argv, envp);
    }
    exit(0);
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
        default:
        {
            puts("Unknown option!\n");
            exit(0);
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