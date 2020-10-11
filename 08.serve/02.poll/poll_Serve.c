#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

#define MAXFD 382
#define PORT 8000
#define M_FILE 1024

int main(void)
{
    int maxi = 0, listenfd, confd;
    int i =0;
    int ready;
    socklen_t len;
    char buf[M_FILE];
    struct sockaddr_in seraddr, clientaddr;
    struct pollfd client[MAXFD];

    listenfd = socket( AF_INET, SOCK_STREAM, 0);
    bzero( &seraddr, sizeof(seraddr) );
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 20);

    client[0].fd = listenfd;
    client[0].events = POLLIN;
    for(i=1;i<MAXFD; i++)
        client[i].fd = -1;
    
    for(;;)
    {
        
        ready = poll(client, maxi+1, -1);
       
        if(client[0].revents & POLLIN )
        {
            
            len = sizeof(clientaddr);
            confd = accept(listenfd, (struct sockaddr*)&clientaddr, &len );
            
            for(i=1; i<MAXFD; i++)
            {
                if(client[i].fd == -1)
                {    
                    client[i].fd = confd;
                    break;
                }
                    
            }
            
            if(i == MAXFD)
                printf("open file too many");
            
            client[i].events = POLLIN;
            if(i>maxi)
                maxi = i;
            
            if(--ready<0)
                continue; 
            
        }
        
        for(i=1; i<=maxi; i++)
        {
            //
            if(client[i].fd < 0)
                continue;

            if(client[i].revents & POLLIN)
            {
                //
                int n;

                if( (n= read(client[i].fd, buf, sizeof(buf)))<0 )
                {  
                    perror("read");
                }

                else if(n == 0)
                {    
                    printf("alread close");
                    close(client[i].fd);
                    client[i].fd = -1;
                }
               
                else
                {
                    
                    for(int j=0; j<n; j++)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    //client[i].fd==6;
                    write(client[i].fd, buf, n);
                    
                }
                //
                if(--ready<0)
                    break;
                
            }
            
            
            
        }
    }

    return 0;
}