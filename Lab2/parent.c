#include "func_parent.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[], char *envp[])
{
    printEnv(envp);
    while (processNumber < 100)
    {
        bool cont = false;
        puts("  +:      search by getenv();");
        puts("  *:      search by *envp[];");
        puts("  &:      search by **environ;");
        puts("  q:      exit of parent process.");
        printf("Your choice:    ");
        char ch = 0;
        rewind(stdin);
        scanf("%c", &ch);
        getchar();
        switch (ch)
        {
        case '+':
        case '*':
        case '&':
            break;
        case 'q':
            exit(0);
        default:
            puts("Unknown option!\n");
            cont = true;
            break;
        }
        if (cont)
            continue;
        createCHILD(ch, argv, envp);
    }
    exit(0);
}