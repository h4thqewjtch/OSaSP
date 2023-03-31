#include "header.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct comm commands = {0, 0, 0, 0};
    int arg;
    extern char *optarg;
    extern int optind, optopt;
    while ((arg = getopt(argc, argv, "dlfs")) != -1)
    {
        switch (arg)
        {
        case 'd':
        {
            commands.dir = true;
            break;
        }
        case 'f':
        {
            commands.file = true;
            break;
        }
        case 'l':
        {
            commands.link = true;
            break;
        }
        case 's':
        {
            commands.sort = true;
            break;
        }
        case '?':
        {
            printf("unknown commands:%c\n", optopt);
            break;
        }
        }
    }
    if (argc <= 2 && !commands.dir && !commands.file && !commands.link && !commands.sort)
        commands.dir = commands.file = commands.link = commands.sort = true;
    if (optind < argc)
        dirWalk(argv[optind], commands);
    else
        dirWalk(".", commands);
    return 0;
}