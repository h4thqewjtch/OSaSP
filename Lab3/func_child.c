#include "func_child.h"

//#include <bits/sigaction.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SA_RESTART 0x10000000

struct two_words data={0, 0};
struct pares pr = {0, 0, 0, 0};

int count = 0;
bool able = false;

void atexit_handler()
{
    printf("The child process %d finished the output!\n", getpid());
}

void set_printallow(int signal)
{
    if (signal == SIGUSR1)
        able = true;
    else
        able = false;
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

void timer_handler()
{
    count++;
    if (data.a)
    {
        if (data.b)
            pr._11++;
        else
            pr._10++;
    }
    else
    {
        if (data.b)
            pr._01++;
        else
            pr._00++;
    }
}

void set_timer_handler()
{
    struct sigaction timer_sig;
    memset(&timer_sig, 0, sizeof(timer_sig));
    timer_sig.sa_handler = &timer_handler;
    sigaction(SIGALRM, &timer_sig, NULL);
}

void init_timer(struct itimerval *timer)
{
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = 10000;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = 10000;
    setitimer(ITIMER_REAL, timer, NULL);
}