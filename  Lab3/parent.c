#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

void handler(int); // ANSI C

int main(int argc, char *argv[], char *envp[])
{
    __pid_t *pid_p = NULL;
    pid_p = (__pid_t *)malloc(256 * sizeof(__pid_t));
    int size = 0;
    while (1)
    {
        puts("  +:            create a child process;");
        puts("  -:            delete the last child process;");
        puts("  l:            show the list of processes;");
        puts("  k:            delete all child processes;");
        puts("  s:            disable all statistics;");
        puts("  g:            able all statistics;");
        puts("  p<num>:       able statistic for child process C_<num>;");
        puts("  q:            delete all child processes and exit of parent process.");
        printf("Your choice: ");
        char var = 0;
        rewind(stdin);
        scanf("%c", &var);
        switch (var)
        {
        case '+':
            system("clear");
            puts("Create a child process\n");
            pid_p[size] = fork();
            size++;
            if (*pid_p == -1)
            {
                printf("Error occured. Error code - %d\n", errno);
                exit(errno);
            }
            if (*pid_p == 0)
            {
                printf("Child process created. Please wait...\n");
                char *args[] = {argv[1], argv[2], (char *)0};
                if (execve(argv[1], args, (void *)0) == -1)
                {
                    printf("Error occured. Error code - %d\n", errno);
                    exit(errno);
                }
            }
            break;
        case '-':
            system("clear");
            puts("Delete the last child process\n");
            if (kill(pid_p[size], SIGKILL) == -1)
            {
                printf("Error occured. Error code - %d\n", errno);
                exit(errno);
            }
            printf("    There are %d child process\n", size + 1);
            size--;
            break;
        case 'l':
            system("clear");
            puts("The list of processes:\n");
            printf("    Parent process pid:         %d\n", (int)getppid());
            for (int i = 0; i <= size; i++)
                printf("    Child process %2d pid:      %2d\n", i, pid_p[i]);
            break;
        case 'k':
            system("clear");
            puts("Delete all child processes\n");
            for (; size >= 0; size++)
                if (kill(pid_p[size], SIGKILL) == -1)
                {
                    printf("Error occured. Error code - %d\n", errno);
                    exit(errno);
                }
            break;
        case 's':
            printf("Enter <num>(0 - %d) to disable statistic for C_<num>\n", size);
            printf("Enter -1 to disable all statistic\n");
            int number = -1;
            rewind(stdin);
            scanf("%d", &number);
            system("clear");
            if (number > -1 && number <= size)
            {
            }
            else if (number == -1)
            {
            }
            else
                puts("Unknown option!\n");
            break;
        case 'g':
            printf("Enter <num>(0 - %d) to able statistic for C_<num>\n", size);
            printf("Enter -1 to able all statistic\n");
            int number = -1;
            rewind(stdin);
            scanf("%d", &number);
            system("clear");
            if (number > -1 && number <= size)
            {
                printf("Able statistic for child process C_%d\n", number);
            }
            else if (number == -1)
            {
                puts("Able all statistics\n");
            }
            else
                puts("Unknown option!\n");
            break;
        case 'p':
            printf("Enter <num>(0 - %d) to able statistic for C_<num>\n", size);
            break;
        case 'q':
            system("clear");
            puts("Delete all child processes and exit\n");
            for (; size >= 0; size++)
                if (kill(pid_p[size], SIGKILL) == -1)
                {
                    printf("Error occured. Error code - %d\n", errno);
                    exit(errno);
                }
            exit(0);
            break;
        default:
            puts("Unknown option!\n");
            break;
        }
    }
    exit(0);
}

void handler(int SIGNUM)
{
    switch (SIGNUM)
    {
    case 0:
        break;
    default:
        break;
    }
}