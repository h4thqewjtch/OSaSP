#include <stdbool.h>
#include <sys/time.h>

extern struct two_words
{
    int a;
    int b;
} data;

extern struct pares 
{
    int _00;
    int _01;
    int _10;
    int _11;
} pr;

extern int count;
extern bool able;

void atexit_handler();
void init_sigaction();
void set_timer_handler();
void init_timer(struct itimerval *);