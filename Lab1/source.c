#include "header.h"

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void dirWalk(char *dirName, struct comm commands)
{
    DIR *dir;
    dir = opendir(dirName);
    if (dir == NULL)
    {
        puts("Parameter <<dirName>> is wrong!\n");
        return;
    }
    char **walker = NULL;
    walker = (char **)malloc(1024 * sizeof(char *));
    for (int i = 0; i < 1024; i++)
        walker[i] = (char *)malloc(256 * sizeof(char));
    int size = 0;
    struct dirent *data;
    while ((data = readdir(dir)) != NULL)
    {
        char path[257];
        if (strcmp(data->d_name, ".") == 0 ||
            strcmp(data->d_name, "..") == 0)
            continue;
        if (data->d_type == 8 && commands.file)
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            walker[size] = (char *)malloc(257 * sizeof(char));
            strcpy(walker[size], path);
            size++;
        }
        if (data->d_type == 10 && commands.link)
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            walker[size] = (char *)malloc(257 * sizeof(char));
            strcpy(walker[size], path);
            size++;
        }
        if (data->d_type == 4)
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            walker[size] = (char *)malloc(257 * sizeof(char));
            strcpy(walker[size], path);
            size++;
        }
    }
    closedir(dir);
    if (commands.sort)
        qsort((void *)walker, size, sizeof(char *), compare);
    struct stat info;
    for (int i = 0; i < size; i++)
    {
        stat(walker[i], &info);
        if ((info.st_mode & __S_IFMT) == __S_IFDIR)
        {
            if (commands.dir)
                puts(walker[i]);
            dirWalk(walker[i], commands);
        }
        else
            puts(walker[i]);
    }
    for (int i = 0; i < size; i++)
        free(walker[i]);
    free(walker);
}

int compare(const void *arg1, const void *arg2)
{
    return strcmp(*(char **)arg1, *(char **)arg2);
}