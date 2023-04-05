#include "func_parent.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    while (1)
    {
        printf("Enter:\n");
        printf("  [+]:            create a child process;\n");
        printf("  [-]:            delete the last child process;\n");
        printf("  [k]:            delete all child processes;\n");
        printf("  [l]:            show the list of processes;\n");
        printf("  [s]:            disable statistics;\n");
        printf("  [g]:            able statistics;\n");
        printf("  [p]:            able statistic for child process C_<num>;\n");
        printf("  [q]:            delete all child processes and exit of parent process.\n");
        printf("Your choice: ");
        char var = 0;
        rewind(stdin);
        scanf("%c", &var);
        getchar();
        exec_choice(var);
    }
    exit(0);
}