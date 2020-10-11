#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <time.h>
#include <errno.h>

#define MAX_EVENTS 1024
#define BUFSIZE 4096
#define PORT 8888

struct myevent_s
{
    int fd;
    int events;
    void *arg;
    void (*call_back)(int fd, int events, void *arg);
    int status;
    char buf[BUFSIZE];
    int len;
    long last_time;
};

int efd;
struct myevent_s g_events[MAX_EVENTS];

void recvdata(int fd, int events, void *arg);
void senddata(int fd, int evnets, void *arg);

void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd, int events, void *arg), void *arg)
{
    ev->fd = fd;
    ev->events = 0;
    ev->call_back = call_back;
    ev->status = 0;
    ev->arg = arg;
    if(ev->len <= 0)
    {
        memset(ev->buf, 0, sizeof(ev->buf));
        ev->len = 0;
    }
    ev->last_time = time(NULL);
}

void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event tem;
    tem.data.ptr = ev;
    tem.events = ev->events = events;
    if(ev->status == 0)
    {
        ev->status = 1;
    }
    if(epoll_ctl(efd, EPOLL_CTL_ADD, ev->fd, &tem)<0)
        printf("event add failed [fd=%d], events[%d]\n", ev->fd, events);
    else
        printf("event add ok [fd=%d], events[%d]\n", ev->fd, events);
    
}

void eventdel(int efd, struct  myevent_s *ev)
{
    struct epoll_event tem = {0, {0}};
    if(ev->status==0)
    {
        return;
    }
    ev->status = 0;
    tem.data.ptr = NULL;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &tem);
    return;
}

void acceptconn(int lfd, int events, void *arg)
{
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int clientfd, i;
    clientfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);

    for(i=0; i<MAX_EVENTS; i++)
    {
        if(g_events[i].status == 0)
            break;
    }
    if(i==MAX_EVENTS)
    {
        printf("connect alrealy max");
        return;
    }
    int flag=0;
    if((flag = fcntl(clientfd, F_SETFL, O_NONBLOCK))<0)
    {
        printf("set nonblocking failed");
        return;
    }
    eventset(&g_events[i], clientfd, recvdata, &g_events[i]);
    eventadd(efd, EPOLLIN, &g_events[i]);

    printf("new connet[%s], pos[%d]", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
}

void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = recv(fd, ev->buf, sizeof(ev->buf), 0);
    eventdel(efd, ev);
    if(len>0)
    {
        ev->len = len;
        ev->buf[len] = '\0';
        printf("C[%d]:%s\n", fd, ev->buf);

        eventset(ev, fd, senddata, ev);
        eventadd(efd, EPOLLOUT, ev);
    }
    
}

void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s*)arg;
    int len = send(fd, ev->buf, ev->len, 0);
    eventdel(efd, ev);

    if(len > 0)
    {
        printf("send[fd=%d], len[%d]", fd, len);
        eventset(ev, fd, recvdata, ev);
        eventadd(efd, EPOLLIN, ev);
    }
    else
    {
        close(ev->fd);
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }
    
}

void initlistensockt(int efd, short port)
{
    struct sockaddr_in sockaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    listen(sockfd, 20);
    eventset(&g_events[MAX_EVENTS], sockfd, acceptconn, &g_events[MAX_EVENTS]);
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);

}

int main()
{
    int i;
    efd = epoll_create(MAX_EVENTS+1);
    if(efd<=0)
        printf("creat err: %s", strerror(errno));
    initlistensockt(efd, PORT);

    struct epoll_event events[MAX_EVENTS+1];

    while(1){

    int epo = epoll_wait(efd, events, MAX_EVENTS+1, 1000);
    for(i=0; i<epo; i++)
    {
        struct myevent_s *ev = (struct myevent_s*)events[i].data.ptr;
        if((events[i].events & EPOLLIN) && (ev->events & EPOLLIN))
        {
            ev->call_back(ev->fd, events[i].events, ev->arg);
        }
        if((events[i].events & EPOLLOUT) && (ev->events && EPOLLOUT))
        {
            ev->call_back(ev->fd, events[i].events, ev->arg);
        }
    }

    }
    return 0;
}