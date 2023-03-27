#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    fprintf(stdout, "Child process begins...\n");
    fprintf(stdout, "   Process name:           %s_%s\n", argv[0], argv[1]);
    fprintf(stdout, "   Pid:                    %d\n", (int)getpid());
    fprintf(stdout, "   PPid:                   %d\n", (int)getppid());
    FILE *file;
    if ((file = fopen(argv[2], "r")) == NULL)
    {
        printf("Cannot open file.\n");
        exit(1);
    }
    char *var = NULL;
    if (!(var = (char *)malloc(256 * sizeof(char))))
    {
        printf("Memory allocation error!\n");
        exit(1);
    }
    int i;
    char c;
    while (!feof(file))
    {
        i = 0;
        while (var[i])
            var[i++] = '\0';
        i = 0;
        while ((c = getc(file)) != '\n')
            var[i++] = c;
        if (feof(file))
            break;
        if (!strcmp(var, "SHELL"))
            fprintf(stdout, "   SHELL:                  %s\n", getenv("SHELL"));
        else if (!strcmp(var, "HOME"))
            fprintf(stdout, "   HOME:                   %s\n", getenv("HOME"));
        else if (!strcmp(var, "HOSTNAME"))
            fprintf(stdout, "   HOSTNAME:               %s\n", getenv("HOSTNAME"));
        else if (!strcmp(var, "LOGNAME"))
            fprintf(stdout, "   LOGNAME:                %s\n", getenv("LOGNAME"));
        else if (!strcmp(var, "LANG"))
            fprintf(stdout, "   LANG:                   %s\n", getenv("LANG"));
        else if (!strcmp(var, "TERM"))
            fprintf(stdout, "   TERM:                   %s\n", getenv("TERM"));
        else if (!strcmp(var, "USER"))
            fprintf(stdout, "   USER:                   %s\n", getenv("USER"));
        else if (!strcmp(var, "LC_COLLATE"))
            fprintf(stdout, "   LC_COLLATE:             %s\n", getenv("LC_COLLATE"));
        else if (!strcmp(var, "PATH"))
            fprintf(stdout, "   PATH:                   %s\n", getenv("PATH"));
        else
            fprintf(stdout, "   Unknown Variable!\n");
    }
    free(var);
    fclose(file);
    exit(0);
}
