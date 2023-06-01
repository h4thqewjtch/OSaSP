#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define nameof(name) #name

#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

int main(int argc, char *argv[])
{
    int sockfd = 0, nread = 0, len = 0;
    char symbol = 0;
    struct sockaddr_in serv_addr;

    char data[8192];
    char response[8192];
    char curDir[1024];
    memset(curDir, 0, 1024);
    bool slash = 0;
    // if (argc != 2)
    // {
    //     printf("\n Usage: %s <ip of server> \n", argv[0]);
    //     return 1;
    // }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERROR_HANDLER("socket", nameof(client));
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        ERROR_HANDLER("inet_pton", nameof(client));
        close(sockfd);
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        ERROR_HANDLER("connect", nameof(client));
        close(sockfd);
        return -1;
    }


//////////////////////////////////////////////////////////////////////////////ДОРАБОТАТЬ ВЫВОД КОМАНД ИЗ ФАЙЛА//ДОБАВИТЬ КОМАНДНУЮ СТРОКУ////////////////////////////////////////////////////////////////////////



    while (1)
    {
        while (nread = recv(sockfd, response, 8192, 0))
        {
            if (nread == -1)
            {
                ERROR_HANDLER("recv", nameof(client));
                close(sockfd);
                return -1;
            }
            len = strlen(response);
            //printf("%d\n", len);
            if (response[len - 1] == '\n')
            {

                break;
            }
            else if (nread == 0)
            {
                break;
            }
        }
        if (!strncmp(data, "CD", 2))
        {
            memset(curDir, 0, 1024);
            for (int i = 0; i < len; i++)
            {
                if (response[i] == ';' || response[i] == '\n')
                {
                    slash = 1;
                    response[i] = '\n';
                }
                if (!slash)
                {
                    curDir[i] = response[i];
                }
            }
            slash = 0;
            if (!strcmp(curDir, "/home/h4thqewjtch/Libs/"))
            {
                memset(curDir, 0, 1024);
            }
        }
        else if (!strncmp(data, "LIST", 4))
        {
            for (int i = 0; i < len; i++)
            {
                if (response[i] == ';')
                {
                    response[i] = '\n';
                }
            }
            response[len - 1] = '\n';
            printf("%s", response);
        }
        else
        {
            response[len - 1] = '\n';
            printf("%s", response);
            if (!strncmp(response, "BYE", 3) || !strncmp(response, "Too", 3))
            {
                break;
            }
        }

        memset(response, 0, 8192);
        memset(data, 0, 8192);

        printf("%s>", curDir);
        rewind(stdin);
        fgets(data, 8192, stdin);
        len = strlen(data);
        //printf("%d\n", len);
        data[len - 1] = '\n';
        if (send(sockfd, data, len, 0) == -1)
        {
            ERROR_HANDLER("send", nameof(client));
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;
}