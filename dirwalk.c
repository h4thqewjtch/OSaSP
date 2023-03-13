#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

struct opt
{
    char dir;
    char file;
    char link;
    char sort;
};

void Menu(int, char **);
void DirWalk(char *, struct opt);
int Compare(const void *, const void *);

int main(int argc, char *argv[])
{
    Menu(argc, argv);
    return 0;
}

void Menu(int argc, char *argv[])
{
    struct opt option;
    int arg;
    extern char *optarg;
    extern int optind, optopt;
    while ((arg = getopt(argc, argv, "dlfs")) != -1)
    {
        switch (arg)
        {
        case 'd':
        {
            option.dir = arg;
            break;
        }
        case 'f':
        {
            option.file = arg;
            break;
        }
        case 'l':
        {
            option.link = arg;
            break;
        }
        case 's':
        {
            option.sort = arg;
            break;
        }
        case '?':
        {
            printf("unknown option:%c\n", optopt);
            break;
        }
        }
    }
    if (argc <= 2 && option.dir != 'd' && option.file != 'f' && option.link != 'l' && option.sort != 's')
    {
        option.dir = 'd';
        option.file = 'f';
        option.link = 'l';
        option.sort = 's';
    }
    if (optind < argc)
        DirWalk(argv[optind], option);
    else
        DirWalk(".", option);
}

void DirWalk(char *dirName, struct opt option)
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
    int size = 0;
    struct dirent *data;
    while ((data = readdir(dir)) != NULL)
    {
        char path[1024];
        if (strcmp(data->d_name, ".") == 0 ||
            strcmp(data->d_name, "..") == 0)
            continue;
        if (data->d_type == 8 && option.file == 'f')
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            walker[size] = (char *)malloc(1024 * sizeof(char));
            strcpy(walker[size], path);
            size++;
        }
        if (data->d_type == 10 && option.link == 'l')
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            walker[size] = (char *)malloc(1024 * sizeof(char));
            strcpy(walker[size], path);
            size++;
        }
        if (data->d_type == 4)
        {
            snprintf(path, sizeof(path), "%s/%s", dirName, data->d_name);
            if (option.dir == 'd')
            {
                walker[size] = (char *)malloc(1024 * sizeof(char));
                strcpy(walker[size], path);
                size++;
            }
            DirWalk(path, option);
        }
    }
    closedir(dir);
    if (option.sort == 's')
        qsort(walker, size, sizeof(char *), (int (*)(const void *, const void *))Compare);
    for (int i = 0; i < size; i++)
        puts(walker[i]);
    for (int i = 0; i < size; i++)
        free(walker[i]);
    free(walker);
}

int Compare(const void *str1, const void *str2)
{
    return strcmp((char *)str1, (char *)str2);
}