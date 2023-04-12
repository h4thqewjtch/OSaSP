#include <sys/wait.h>

void exec_choice(char);
void create_child();
void kill_process();
void list_processes();
void send_signal(pid_t, int);
void set_timer_handler();
void timer_handler();
void init_timer();