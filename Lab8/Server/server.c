#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#define nameof(name) #name

#define ERROR_HANDLER(func, name)                         \
    {                                                     \
        perror(func);                                     \
        fprintf(stderr, "Error code: %d\n", errno);       \
        fprintf(stderr, "Error environment: %s\n", name); \
    }

int main(int argc, char *argv[])
{

    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;

    char sendBuff[1025];
    time_t ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        ERROR_HANDLER("socket", nameof(main));
        return -1;
    }
    if (memset(&serv_addr, '0', sizeof(serv_addr)) == NULL)
    {
        ERROR_HANDLER("memset", nameof(main));
        return -1;
    }
    if (memset(sendBuff, '0', sizeof(sendBuff)) == NULL)
    {
        ERROR_HANDLER("memset", nameof(main));
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        ERROR_HANDLER("bind", nameof(main));
        return -1;
    }

    if (listen(listenfd, 20) == -1)
    {
        ERROR_HANDLER("listen", nameof(main));
        return -1;
    }

    while (1)
    {
        if ((connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1)
        {
            ERROR_HANDLER("accept", nameof(main));
            return -1;
        }

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        if (write(connfd, sendBuff, strlen(sendBuff)) == -1)
        {
            ERROR_HANDLER("write", nameof(main));
            return -1;
        }

        close(connfd);
        sleep(1);
    }
}