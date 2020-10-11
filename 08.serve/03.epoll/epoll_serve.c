#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define PORT 8100
#define MAX_FILE 1024
#define MAXI 388

int main()
{
    int listenfd, confd, maxi, i, j, epo, ready;
    int client[MAXI];
    socklen_t len;
    ssize_t n;
    struct sockaddr_in seraddr, clientaddr;
    char buf[MAX_FILE];
    struct epoll_event tep, teps[MAXI];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 128);

    for(int i=0; i<MAXI; i++)
        client[i] = -1;
    maxi = -1;

    epo = epoll_create(MAXI);
    tep.events = EPOLLIN;
    tep.data.fd = listenfd;
    epoll_ctl(epo, EPOLL_CTL_ADD, listenfd, &tep);

    for(; ;)
    {
        ready = epoll_wait(epo, teps, MAXI, -1);
        if(ready < 1)
            perror("epoll_wait");
        for(i=0; i<ready; i++)
        {
            if(!(teps[i].events & EPOLLIN))
                continue;
            if(teps[i].data.fd == listenfd)
            {
                len = sizeof(clientaddr);
                confd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);

                for(j=0; j<MAXI; j++)
                {
                    if(client[j] == -1){

                        client[j] = confd;
                        break;
                    }
                } 
                if(j > maxi)
                    maxi =j;
                
                tep.events = EPOLLIN;
                tep.data.fd = confd;
                epoll_ctl(epo, EPOLL_CTL_ADD, confd, &tep);
                                
            }
            else
            {
                n = read(teps[i].data.fd, buf, sizeof(buf));
                /*close
                for(j=0; j<MAXI;j++)
                {
                    if(client[j] == teps[i].data.fd)
                        client[j] = -1;
                        break;
                }
                epoll_ctl(epo, EPOLL_CTL_DEL, teps[i].data.fd, NULL);
                */
               for(int l=0; l<MAX_FILE; l++)
                    buf[l] = toupper(buf[l]);
                write(teps[i].data.fd, buf, n);
            }
            
        }
    }

    return 0;
}