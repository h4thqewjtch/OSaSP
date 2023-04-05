#include "func_parent.h"

// #include <bits/sigaction.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

pid_t pids[128];
int max_ind = 0;

void exec_choice(char var)
{
    switch (var)
    {
    case '+':
    {
        system("clear");
        printf("Create a child process\n");
        create_child();
        break;
    }
    case '-':
    {
        system("clear");
        printf("Delete the last child process\n");
        kill_process();
        printf("    There are %d child process\n", max_ind);
        break;
    }
    case 'k':
    {
        system("clear");
        printf("Delete all child processes\n");
        kill_all();
        break;
    }
    case 'l':
    {
        system("clear");
        printf("The list of processes:\n");
        list_processes();
        break;
    }
    case 's':
    {
        system("clear");
        printf("Enter:\n");
        printf("              [-1]:          disable all statistic\n");
        printf("    <num>(0 - %2d):          disable statistic for C_<num>\n", max_ind - 1);
        int number = -1;
        rewind(stdin);
        scanf("%d", &number);
        getchar();
        if (number > -1 && number < max_ind)
        {
            printf("Disable statistic for child process C_%d\n", number);
            send_signal(pids[number], SIGUSR2);
        }
        else if (number == -1)
        {
            printf("Disable all statistics\n");
            for (int i = 0; i < max_ind; i++)
                send_signal(pids[i], SIGUSR2);
        }
        else
            printf("Unknown option!\n");
        break;
    }
    case 'g':
    {
        system("clear");
        printf("Enter:\n");
        printf("              [-1]:          able all statistic\n");
        printf("    <num>(0 - %2d):          able statistic for C_<num>\n", max_ind - 1);
        int number;
        rewind(stdin);
        scanf("%d", &number);
        getchar();
        if (number > -1 && number < max_ind)
        {
            printf("Able statistic for child process C_%d\n", number);
            send_signal(pids[number], SIGUSR1);
        }
        else if (number == -1)
        {
            printf("Able all statistics\n");
            for (int i = 0; i < max_ind; i++)
                send_signal(pids[i], SIGUSR1);
        }
        else
            printf("Unknown option!\n");
        break;
    }
    case 'p':
    {
        system("clear");
        printf("Enter:\n");
        printf("    <num>(0 - %d):          able statistic for C_<num>\n", max_ind - 1);
        int number;
        rewind(stdin);
        scanf("%d", &number);
        getchar();
        if (number > -1 && number < max_ind)
        {
            struct itimerval timer;
            printf("Able statistic for child process C_%d\n", number);
            send_signal(pids[number], SIGUSR1);
            set_timer_handler();
            init_timer(&timer);
        }
        else
            printf("Unknown option!\n");
        break;
    }
    case 'q':
    {
        printf("Delete all child processes and exit\n");
        for (int i = 0; i < max_ind; i++)
            send_signal(pids[i], SIGKILL);
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

void create_child()
{
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
            printf("Error code: %d\n", errno);
            exit(errno);
        }
    }
    else if (pid > 0)
    {
        pids[max_ind] = pid;
        max_ind++;
    }
}

void kill_process()
{
    max_ind--;
    send_signal(pids[max_ind], SIGKILL);
}

void kill_all()
{
    for (int i = 0; i < max_ind; i++)
    {
        send_signal(pids[i], SIGKILL);
        pids[i] = 0;
    }
    max_ind = 0;
}

void list_processes()
{
    printf("    Parent process pid:         %d\n", (int)getppid());
    for (int i = 0; i < max_ind; i++)
        printf("    Child process №%2d pid:      %d\n", i, pids[i]);
}

void send_signal(pid_t pid, int SIGNUM)
{
    if (kill(pid, SIGNUM) == -1)
    {
        printf("Error code: %d\n", errno);
        exit(errno);
    }
}

void init_timer(struct itimerval *timer)
{
    timer->it_value.tv_sec = 5;
    timer->it_value.tv_usec = 0;
    timer->it_interval.tv_sec = 5;
    timer->it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, timer, NULL);
}

void set_timer_handler()
{
    struct sigaction timer_sig;
    memset(&timer_sig, 0, sizeof(timer_sig));
    timer_sig.sa_handler = &timer_handler;
    sigaction(SIGALRM, &timer_sig, NULL);
}

void timer_handler()
{
    for (int i = 0; i < max_ind; i++)
        send_signal(pids[i], SIGUSR1);
}