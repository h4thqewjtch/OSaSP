#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_CLIENTS 16

#define nameof(name) #name

#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

//////////////////////////////////////////////////////////////////////////////ДОРАБОТАТЬ ВЫВОД КОМАНД ИЗ ФАЙЛА//ДОБАВИТЬ КОМАНДНУЮ СТРОКУ////////////////////////////////////////////////////////////////////////

int clientCount = 0;

char rootDir[1024];

char data[8192];
int size = 0;

struct request_f
{
    char str[8192];
} rec;

void dirWalk(char *dirName)
{
    DIR *d;
    d = opendir(dirName);
    if (d == NULL)
    {
        puts("Parameter <<dirName>> is wrong!\n");
        return;
    }
    struct dirent *dirent;
    char name[1024];
    while ((dirent = readdir(d)) != NULL)
    {
        if (!strcmp(dirent->d_name, ".") ||
            !strcmp(dirent->d_name, ".."))
            continue;
        if (dirent->d_type == 8) // file
        {
            snprintf(name, sizeof(name), "%s", dirent->d_name);
            if (size + strlen(name) >= 8192)
                break;
            for (int i = 0; i < strlen(name); i++)
            {
                data[size] = name[i];
                size++;
            }
            data[size] = ';';
            size++;
        }
        if (dirent->d_type == 10) // symLink
        {
            char buffer[256];
            readlink(dirent->d_name, buffer, 256);
            snprintf(name, sizeof(name), "%s-->%s", dirent->d_name, buffer);
            if (size + strlen(name) >= 8192)
                break;
            for (int i = 0; i < strlen(name); i++)
            {
                data[size] = name[i];
                size++;
            }
            data[size] = ';';
            size++;
        }
        if (dirent->d_type == 4) // directory
        {
            snprintf(name, sizeof(name), "%s/", dirent->d_name);
            if (size + strlen(name) >= 8192)
                break;
            for (int i = 0; i < strlen(name); i++)
            {
                data[size] = name[i];
                size++;
            }
            data[size] = ';';
            size++;
        }
    }
    data[size] = '0';
    closedir(d);
    // struct stat info;
    // for (int i = 0; i < size; i++)
    // {
    //     stat(walker[i], &info);
    //     if ((info.st_mode & __S_IFMT) == __S_IFDIR)
    //     {
    //         dirWalk(walker[i]);
    //     }
    //     else
    //         puts(walker[i]);
    // }
}

void client_thread(void *fd)
{

    int connfd = *(int *)fd, nread = 0, len = 0;

    char dir[1024];
    strcpy(dir, rootDir);

    int count = 0;

    while (1)
    {
        memset(data, 0, strlen(data));
        while (nread = recv(connfd, data, 8192, 0))
        {
            if (nread == -1)
            {
                ERROR_HANDLER("recv", nameof(server));
                close(connfd);
                return;
            }
            len = strlen(data);
            // printf("%d\n", len);
            if (data[len - 1] == '\n')
            {
                break;
            }
            else if (nread == 0)
            {
                break;
            }
        }
        int fd = 0;
        int nread = 0;
        int len = 0;

        char INFO[] = {"Welcome to study server\n"};
        char QUIT[] = {"BYE\n"};

        if (!strncmp(data, "@", 1))
        {
            char filePath[8192];
            memset(filePath, 0, 8192);
            strcat(filePath, dir);
            strcat(filePath, "/");
            char fileName[1024];
            memset(fileName, 0, 1024);
            for (int i = 0; i < strlen(data) - 2; i++)
            {
                fileName[i] = data[i + 1];
            }
            strcat(filePath, fileName);
            // printf("Path : %s", filePath);
            if ((fd = open(filePath, O_RDWR)) == -1)
            {
                ERROR_HANDLER("open", nameof(main));
                if (send(connfd, "No such file or directory\n", strlen("No such file or directory\n"), 0) == -1)
                {
                    ERROR_HANDLER("send", nameof(server));
                    close(connfd);
                }
                continue;
            }
            struct flock fLock;
            fLock.l_type = F_RDLCK;
            fLock.l_whence = SEEK_SET;
            fLock.l_start = 0;
            fLock.l_len = 0;
            if (fcntl(fd, F_SETLK, &fLock) == -1)
            {
                ERROR_HANDLER("fcntl", nameof(server));
                return;
            }
            char symbol = 0;
            lseek(fd, count * sizeof(struct request_f), SEEK_SET);
            if (!read(fd, &symbol, sizeof(char)))
            {
                if (send(connfd, "End of file\n", strlen("End of file\n"), 0) == -1)
                {
                    ERROR_HANDLER("send", nameof(server));
                    close(connfd);
                }
                count = 0;
                continue;
            }
            else
            {
                lseek(fd, -1, SEEK_CUR);
            }
            if (read(fd, &rec, sizeof(struct request_f)) == -1)
            {
                ERROR_HANDLER("read", nameof(server));
                return;
            }
            printf("%s\n", rec.str);
            count++;
            printf("count: %d\n", count);
            strcpy(data, rec.str);
            fLock.l_type = F_UNLCK;
            if (fcntl(fd, F_SETLKW, &fLock) == -1)
            {
                ERROR_HANDLER("fcntl", nameof(server));
                return;
            }
            close(fd);
        }
        if (!strncmp(data, "INFO", 4))
        {
            // printf("!!!\n");
            len = strlen(INFO);
            if (send(connfd, INFO, len, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            memset(data, 0, 8192);
        }
        else if (!strncmp(data, "ECHO", 4))
        {
            // printf("!!!\n");
            len = strlen(data);
            for (int i = 0; i < len - 5; i++)
            {
                data[i] = data[i + 5];
            }
            if (send(connfd, data, len - 5, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            memset(data, 0, 8192);
        }
        else if (!strncmp(data, "LIST", 4))
        {
            // printf("!!!\n");
            //  for (int i = 0; i < strlen(dir); i++)
            //  {
            //      data[size] = dir[i];
            //      size++;
            //  }
            //  data[size] = '/';
            //  size++;
            //  data[size] = ';';
            //  size++;
            // printf("%s", dir);
            dirWalk(dir);
            len = strlen(data);
            data[len - 1] = '\n';
            if (send(connfd, data, len, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            memset(data, 0, 8192);
            size = 0;
        }
        else if (!strncmp(data, "CD", 2))
        {

            // printf("!!!\n");
            char name[1024];
            memset(name, 0, 1024);
            len = strlen(data);
            for (int i = 0; i < len - 4; i++)
            {
                name[i] = data[i + 3];
            }
            if (!strncmp(name, "..", 2))
            {
                // printf("!\n");
                // printf("rootDir:%s", rootDir);
                // printf(" dir:%s\n", dir);
                if (strcmp(dir, rootDir) != 0)
                {
                    // printf("!\n");
                    DIR *d = NULL;
                    d = opendir("..");
                    if (d == NULL)
                    {
                        if (send(connfd, "CD argument is wrong\n", strlen("CD argument is wrong\n"), 0) == -1)
                        {
                            ERROR_HANDLER("send", nameof(server));
                            close(connfd);
                            memset(data, 0, 8192);
                            return;
                        }
                        continue;
                    }
                    closedir(d);

                    bool count = 0;
                    for (int i = strlen(dir) - 1; i > 0; i--)
                    {
                        if (dir[i] == '/')
                        {
                            dir[i] = 0;
                            count++;
                            if (count == 1)
                            {
                                break;
                            }
                        }
                        dir[i] = 0;
                    }
                    // printf("!!\n");
                    // printf("dir in .. : %s", dir);
                    count = 0;
                    if (strlen(name) > 2)
                    {
                        char n[1024];
                        memset(n, 0, 1024);
                        for (int i = 0; i < strlen(name) - 3; i++)
                        {
                            n[i] = name[i + 3];
                        }
                        dir[strlen(dir)] = '/';
                        strncat(dir, n, 1024);
                        d = opendir(dir);
                        if (d == NULL)
                        {
                            if (send(connfd, "CD argument is wrong\n", strlen("CD argument is wrong\n"), 0) == -1)
                            {
                                ERROR_HANDLER("send", nameof(server));
                                close(connfd);
                                memset(data, 0, 8192);
                                return;
                            }
                            continue;
                        }
                        closedir(d);
                    }
                }
            }
            else
            {
                dir[strlen(dir)] = '/';
                strncat(dir, name, 1024);
                // printf("dir: %s\n", dir);
                DIR *d = NULL;
                d = opendir(dir);
                if (d == NULL)
                {
                    if (send(connfd, "CD argument is wrong\n", strlen("CD argument is wrong\n"), 0) == -1)
                    {
                        ERROR_HANDLER("send", nameof(server));
                        close(connfd);
                        memset(data, 0, 8192);
                        return;
                    }
                    continue;
                }
                closedir(d);
            }

            for (int i = 0; i < strlen(dir); i++)
            {
                data[size] = dir[i];
                size++;
            }
            data[size] = '/';
            size++;
            data[size] = ';';
            size++;
            int len = strlen(data);
            data[len - 1] = '\n';
            // printf("%s\n", data);
            if (send(connfd, data, len, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            memset(data, 0, 8192);
            size = 0;
        }
        else if (!strncmp(data, "QUIT", 4))
        {
            // printf("!!!\n");
            int len = strlen(QUIT);
            if (send(connfd, QUIT, len, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            clientCount--;
            break;
        }
        else
        {
            // printf("!!!\n");
            len = strlen(data);
            for (int i = 0; i < len - 5; i++)
            {
                data[i] = data[i + 5];
            }

            if (send(connfd, "ERROR\n", strlen("ERROR\n"), 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return;
            }
            memset(data, 0, 8192);
        }
    }
}

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, clientSocket[MAX_CLIENTS];
    struct sockaddr_in serv_addr;

    char INFO[] = {"Welcome to study server\n"};

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        ERROR_HANDLER("socket", nameof(server));
        return -1;
    }
    if (memset(&serv_addr, 0, sizeof(serv_addr)) == NULL)
    {
        ERROR_HANDLER("memset", nameof(server));
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(8080);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        ERROR_HANDLER("bind", nameof(server));
        return -1;
    }
    if (listen(listenfd, 100) == -1)
    {
        ERROR_HANDLER("listen", nameof(server));
        return -1;
    }
    pthread_t clients[MAX_CLIENTS];
    strcpy(rootDir, argv[1]);
    while (1)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            ERROR_HANDLER("accept", nameof(server));
            return -1;
        }

        if (clientCount >= MAX_CLIENTS)
        {
            if (send(connfd, "Too many clients\n", strlen("Too many clients\n"), 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return -1;
            }
        }
        else
        {
            int len = strlen(INFO);
            if (send(connfd, INFO, len, 0) == -1)
            {
                ERROR_HANDLER("send", nameof(server));
                close(connfd);
                return -1;
            }
        }

        if (clientCount < MAX_CLIENTS)
        {
            pthread_create(&clients[clientCount], NULL, (void *)client_thread, &connfd);
            clientCount++;
        }
    }
    return 0;
}