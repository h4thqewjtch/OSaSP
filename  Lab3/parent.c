#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int main(int argc, char *argv[], char *envp[])
{
    pid_t pid_p[1024];
    int max_ind = 0;
    while (1)
    {
        printf("  +:            create a child process;\n");
        printf("  -:            delete the last child process;\n");
        printf("  l:            show the list of processes;\n");
        printf("  k:            delete all child processes;\n");
        printf("  s:            disable all statistics;\n");
        printf("  g:            able all statistics;\n");
        printf("  p:            able statistic for child process C_<num>;\n");
        printf("  q:            delete all child processes and exit of parent process.\n");
        printf("Your choice: ");
        char var = 0;
        rewind(stdin);
        var = getchar();
        getchar();
        switch (var)
        {
        case '+':
        {
            printf("Create a child process\n");
            pid_t pid = fork();
            if (pid == -1)
            {
                printf("Error occured. Error code - %d\n", errno);
                exit(errno);
            }
            if (pid == 0)
            {
                if (execl("./child", "child", NULL) == -1)
                {
                    printf("execve() was failed. Error code - %d\n", errno);
                    exit(errno);
                }
            }
            else if (pid > 0)
            {
                pid_p[max_ind] = pid;
                max_ind++;
            }
            break;
        }
        case '-':
        {
            printf("Delete the last child process\n");
            max_ind--;
            kill(pid_p[max_ind], SIGKILL);
            printf("    There are %d child process\n", max_ind);
            break;
        }
        case 'l':
        {
            printf("The list of processes:\n");
            printf("    Parent process pid:         %d\n", (int)getppid());
            for (int i = 0; i < max_ind; i++)
                printf("    Child process №%2d pid:      %d\n", i + 1, pid_p[i]);
            break;
        }
        case 'k':
        {
            printf("Delete all child processes\n");
            for (; max_ind >= 0; max_ind--)
                kill(pid_p[max_ind], SIGKILL);
            break;
        }
        case 's':
        {
            printf("Enter <num>(0 - %d) to disable statistic for C_<num>\n", max_ind);
            printf("Enter -1 to disable all statistic\n");
            int number = -1;
            rewind(stdin);
            scanf("%d", &number);
            if (number > -1 && number <= max_ind)
            {
                printf("Disable statistic for child process C_%d\n", number);
                kill(pid_p[number], SIGUSR2);
            }
            else if (number == -1)
            {
                printf("Disable all statistics\n");
                kill(0, SIGUSR2);
            }
            else
                printf("Unknown option!\n");
            break;
        }
        case 'g':
        {
            printf("Enter <num>(0 - %d) to able statistic for C_<num>\n", max_ind);
            printf("Enter -1 to able all statistic\n");
            int number;
            rewind(stdin);
            scanf("%d", &number);
            if (number > -1 && number <= max_ind)
            {
                printf("Able statistic for child process C_%d\n", number);
                kill(pid_p[number], SIGUSR1);
            }
            else if (number == -1)
            {
                printf("Able all statistics\n");
                kill(0, SIGUSR1);
            }
            else
                printf("Unknown option!\n");
            break;
        }
        case 'p':
        {
            printf("Enter <num>(0 - %d) to able statistic for C_<num>\n", max_ind);
            int number;
            rewind(stdin);
            scanf("%d", &number);
            if (number > -1 && number <= max_ind)
            {
                printf("Able statistic for child process C_%d\n", number);
                kill(pid_p[number], SIGUSR1);
            }
            else
                printf("Unknown option!\n");
            break;
        }
        case 'q':
        {
            printf("Delete all child processes and exit\n");
            for (; max_ind >= 0; max_ind++)
                kill(pid_p[max_ind], SIGKILL);
            exit(0);
            break;
        }
        default:
        {
            printf("Unknown option!\n");
            break;
        }
        }
    }
    exit(0);
}