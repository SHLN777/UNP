#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <stddef.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_ADDR "serv.socket"
#define CLIE_ADDR "clie.socket"

int main()
{
    int cfd, len, n;
    socklen_t leng;
    struct sockaddr_un servaddr, clientaddr;
    char buf[1024];

    cfd = socket(AF_UNIX, SOCK_STREAM, 0);
    //////////////////////////////////////////////////
    bzero(&clientaddr, sizeof(clientaddr));
    clientaddr.sun_family = AF_UNIX;
    strcpy(clientaddr.sun_path, CLIE_ADDR);

    len = offsetof(struct sockaddr_un, sun_path)+strlen(clientaddr.sun_path);
    unlink(CLIE_ADDR);
    bind(cfd, (struct sockaddr*)&clientaddr, len);

    //////////////////////////////////////////////////////////
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family = AF_UNIX;
    strcpy(servaddr.sun_path, SERV_ADDR);

    len = offsetof(struct sockaddr_un, sun_path)+strlen(servaddr.sun_path);
    
    connect(cfd, (struct sockaddr*)&servaddr, len);

    while(fgets(buf, sizeof(buf), stdin) != NULL)
    {
        write(cfd, buf, strlen(buf));
        n = read(cfd, buf, sizeof(buf));
        
        write(STDOUT_FILENO, buf, n);
        
    }
    close(cfd);

    return 0;
}