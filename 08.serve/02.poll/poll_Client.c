#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8000
#define MAX 1024

int main(void)
{
    struct sockaddr_in seraddr;
    char buf[1024]; 
    int clientfd,n;

    clientfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "10.0.2.15", &seraddr.sin_addr.s_addr);
    seraddr.sin_port = htons(PORT);
    
    connect(clientfd, (struct sockaddr*)&seraddr, sizeof(seraddr));

    while(fgets(buf, MAX, stdin) != NULL)
    {
        write(clientfd, buf, strlen(buf));
        n = read(clientfd, buf, sizeof(buf));
        printf("\n");
        write(STDOUT_FILENO, buf, n);
      

    } 
    
    close(clientfd);
    return 0;
}