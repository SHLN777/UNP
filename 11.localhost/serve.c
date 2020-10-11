#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stddef.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_ADDR "serv.socket"

int main()
{
    int lfd, cfd, len, n;
    int leng;
    struct sockaddr_un servaddr, clientaddr;
    char buf[1024];

    lfd = socket(AF_UNIX, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sun_family=AF_INET;
    strcpy(servaddr.sun_path, SERV_ADDR);

    len = offsetof(struct sockaddr_un, sun_path)+strlen(servaddr.sun_path);
    //unlink(SERV_ADDR);
    bind(lfd, (struct sockaddr*)&servaddr, len);
    listen(lfd, 128);

    while(1){
    leng = sizeof(clientaddr);
    cfd = accept(lfd, (struct sockaddr *)&clientaddr, (socklen_t *)&leng);
  
    while( (n = read(cfd, buf, sizeof(buf))) >0 )
        {
        printf("%s", "asdf");
        for(int i=0; i<n; i++)
            buf[i] = toupper(buf[i]);
        write(cfd, buf, n);
        }
        close(cfd);
    }
    close(lfd);
    return 0;
}