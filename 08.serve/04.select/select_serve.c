#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8111
#define MAXFILE 1024
#define MAX 128

int main()
{
    int listenfd, confd, sockfd;
    int maxfd, maxi, i, ready;
    struct sockaddr_in seraddr, clientaddr;
    char buf[MAXFILE];
    int client[MAX];
    socklen_t len;
    ssize_t n;
    fd_set set, reset;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 128);

    maxfd = listenfd;
    maxi = -1;
    for(i=0; i<MAX; i++)
        client[i] = -1;
    FD_ZERO(&reset);
    FD_SET(listenfd, &reset);
    for(;;)
    {
        set = reset;
        ready = select(maxfd+1, &set, NULL, NULL, NULL);
        if(FD_ISSET(listenfd, &set))
        {
            len = sizeof(clientaddr);
            confd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
            for(i=0; i<MAX; i++)
            {
                if(client[i] == -1)
                {
                    client[i] = confd;
                    break;
                }

            }
            
            FD_SET(confd, &reset);
            if(maxi < i)
                maxi = i;
            if(listenfd < confd)
                maxfd = confd;
            if(--ready == 0)
                continue;
        }
        for(i=0; i<=maxi; i++)
        {
            if((sockfd = client[i])<0)
                continue;
            if(FD_ISSET(sockfd, &set))
            {
                n = read(sockfd, buf, sizeof(buf));
                for(int j =0; j<MAXFILE; j++)
                {
                    buf[j] = toupper(buf[j]);
                }
                write(sockfd, buf, n);
            }
            if(--ready == 0)
                break;
        }
    }

    return 0;
}