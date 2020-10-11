#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000
#define MAX 80

int main(void)
{
    struct sockaddr_in seraddr, clientaddr;
    int listenfd, confd;
    socklen_t len;
    char buf[MAX];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 25);

    while(1)
    {
	
        int n;
        len = sizeof(clientaddr);
        confd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
       	while(1){
		    n = read(confd, buf, sizeof(buf));
        	for(int i=0; i<n; i++)
           	    buf[i] = toupper(buf[i]);
        	write(confd, buf, n);
	}
        
    }

    return 0;
}
