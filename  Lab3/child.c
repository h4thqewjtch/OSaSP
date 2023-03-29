#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <bits/sigaction.h>
#define SA_RESTART 0x10000000

void init_atexit();

volatile struct two_words
{
    int a;
    int b;
} memory;

struct pares
{
    int p_00;
    int p_01;
    int p_10;
    int p_11;
} p = {0, 0, 0, 0};

int count = 0;
int able = 0;

void timer_handler()
{
    count++;
    if (memory.a == 0 && memory.b == 0)
        p.p_00++;
    else if (memory.a == 0 && memory.b == 1)
        p.p_01++;
    else if (memory.a == 1 && memory.b == 0)
        p.p_10++;
    else if (memory.a == 1 && memory.b == 1)
        p.p_11++;
}

void ask_to_print()
{
    kill(getppid(), SIGUSR1);
}

void init_timer(struct itimerval *timer)
{
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = 10000;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = 10000;
    setitimer(ITIMER_REAL, timer, NULL);
}

void set_printallow(int signal)
{
    if (signal == SIGUSR1)
        able = 1;
    else
        able = 0;
}

void init_sigaction()
{
    struct sigaction allow_print, forbid_print;

    allow_print.sa_flags = SA_RESTART;
    allow_print.sa_handler = set_printallow;

    forbid_print.sa_flags = SA_RESTART;
    forbid_print.sa_handler = set_printallow;

    sigaction(SIGUSR2, &allow_print, NULL);
    sigaction(SIGUSR1, &forbid_print, NULL);
}

void set_timer_handler()
{
    struct sigaction timer_sig;
    memset(&timer_sig, 0, sizeof(timer_sig));
    timer_sig.sa_handler = &timer_handler;
    sigaction(SIGALRM, &timer_sig, NULL);
}

void atexit_handler()
{
    printf("child %d is processed.", getpid());
}

void init_atexit()
{
    atexit(atexit_handler);
}

int main(int argc, char *argv[], char *envp[])
{
    struct itimerval timer;
    printf("Start child\n");
    init_atexit();
    init_sigaction();
    set_timer_handler();
    init_timer(&timer);
    ask_to_print();
    while (1)
    {
        memory.a = 0;
        memory.b = 0;
        memory.a = 1;
        memory.b = 1;
        if (count == 100)
        {
            if (able)
            {
                char buffer[256];
                char *str = (char*)&buffer;
                sprintf(buffer, "parent pid = %d\t%s pid = %d\tstatistic - [ 00 - '%2d', 01 - '%2d', 10 - '%2d', 11 - '%2d' ]\n",
                        getppid(), argv[0], getpid(), p.p_00, p.p_01, p.p_10, p.p_11);
                while (*str)
                {
                    fputc(*str++, stdout);
                }
            }
            count = 0;
            p.p_00 = 0;
            p.p_01 = 0;
            p.p_10 = 0;
            p.p_11 = 0;
        }
    }
    return 0;
}