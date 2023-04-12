#include "func_child.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    struct itimerval timer;
    printf("Start child\n");
    atexit(atexit_handler);
    init_sigaction();
    set_timer_handler();
    init_timer(&timer);
    while (1)
    {
        data.a = 0;
        data.b = 0;
        data.a = 1;
        data.b = 1;
        if (count == 100)
        {
            if (able)
            {
                char buffer[256];
                char *str = (char *)&buffer;
                sprintf(buffer,
                        "parent pid = %d\n%s pid  = %d\nstatistic: [ 00 - '%2d' ]\n           [ 01 - '%2d' ]\n           [ 10 - '%2d' ]\n           [ 11 - '%2d' ]\n\n",
                        getppid(), argv[0], getpid(), pr._00, pr._01, pr._10, pr._11);
                while (*str)
                    fputc(*str++, stdout);
            }
            count = 0;
            pr._00 = 0;
            pr._01 = 0;
            pr._10 = 0;
            pr._11 = 0;
        }
    }
    exit(0);
}