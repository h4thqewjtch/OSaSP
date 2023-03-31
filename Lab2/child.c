#include "func_child.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    fprintf(stdout, "Child process begins...\n");
    fprintf(stdout, "   Process name:           %s_%s\n", argv[0], argv[1]);
    fprintf(stdout, "   Pid:                    %d\n", (int)getpid());
    fprintf(stdout, "   PPid:                   %d\n", (int)getppid());
    file_process(argv[2], envp);
    fprintf(stdout, "Child finished!\n");
    exit(0);
}