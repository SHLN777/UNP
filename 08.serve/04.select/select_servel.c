#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

#define PORT 8001
#define MAXFILE 1024
#define MAX

int main()
{
    int listenfd, confd;
    socklen_t len;
    int maxfd, ready;
    fd_set set, reset;
    struct sockaddr_in seraddr, clientaddr;
    char buf[MAXFILE];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 128);

    FD_SET(listenfd, &reset);
    maxfd = listenfd;
    for(;;)
    {
        set = reset;
        ready = select(maxfd+1, &set, NULL, NULL, NULL);
        if(FD_ISSET(listenfd, &set))
        {
            len = sizeof(clientaddr);
            confd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);

            FD_SET(confd, &reset);
            if(listenfd < confd)
            {
                maxfd = confd;
            }
            //
            if(--ready == 0)
                continue;
        }
        //printf("%d, %d, %d\n", listenfd, confd, maxfd);   3,6,6
        for(int i=listenfd+1; i<=maxfd; i++)
        {
            //printf("%d\n", i);
            int n;
            if( FD_ISSET(i, &set) )
            {    
                
                n = read(i, buf, sizeof(buf));
                for(int j=0; j<MAXFILE; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                   
                    write(i, buf, n);
                                      
            }
            
            if(--ready == 0)
                break;
            
            
        }
        
    }


    return 0;
}