#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 8111
#define MAX 1024
#define MAXFILE 1024

int main()
{
    int listenfd, confd,n;
    char buf[MAXFILE];
    string s1("Hello!");
    const char *p;
    vector<int> v;
 
    struct sockaddr_in seraddr, clientaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(PORT);
    bind(listenfd, (struct sockaddr*)&seraddr, sizeof(seraddr));
    listen(listenfd, 128);

    while(1)
    {
        socklen_t m = sizeof(clientaddr);
        confd = accept(listenfd, (struct sockaddr*)&clientaddr, &m);
        while(1)
        {
            n = read(confd, buf, sizeof(buf));
            for(int i=0; i<MAXFILE; i++)
                buf[i] = toupper(buf[i]);
           
            write(confd, buf, n);
        }
        close(confd);
        
    }
    
    
    return 0;

}
