#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    int sockfd = 0, n = 0;
    char symbol = 0;
    char *request;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    if (argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    memset(recvBuff, '0', sizeof(recvBuff));
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        ERROR_HANDLER("socket", nameof(main));
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    puts("Welcome to study server <<myserver>>");
    while (1)
    {
        printf(">");
        rewind(stdin);
        scanf("%s", request);
        if (!strcmp(request, "QUIT"))
        {
            puts("BYE");
            break;
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(5000);

        if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
        {
            ERROR_HANDLER("inet_pton", nameof(main));
            return 1;
        }

        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            ERROR_HANDLER("connect", nameof(main));
            return 1;
        }

        while ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0)
        {
            recvBuff[n] = 0;
            if (fputs(recvBuff, stdout) == EOF)
            {
                ERROR_HANDLER("fputs", nameof(main));
            }
        }

        if (n < 0)
        {
            ERROR_HANDLER("read", nameof(main));
        }
    }
    close(sockfd);
    return 0;
}