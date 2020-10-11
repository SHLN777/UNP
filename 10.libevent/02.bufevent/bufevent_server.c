#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>


void read_cb(struct bufferevent *bev, void *arg)
{
    char buf[1024] = {0};
    bufferevent_read(bev, buf, sizeof(buf));
    printf("client say: %s\n", buf);
    char *p = "我是服务器，已收到数据";
    bufferevent_write(bev, p, strlen(p));
    sleep(1);
}

void write_cb(struct bufferevent *bev, void *arg)
{
    printf("成功写数据给客户端\n");

}

void event_cb(struct bufferevent *bev, short what, void *arg)
{
    if(what & BEV_EVENT_EOF)
    {
        printf("connect closed\n");
    }
    else if(what & BEV_EVENT_ERROR)
    {
        printf("other error\n");
    }
    bufferevent_free(bev);
    printf("资源已被释放\n");
}

void cb_listener(struct evconnlistener *listener, evutil_socket_t fd, 
                                struct sockaddr *addr, int len, void *ptr)
{
    printf("connect new client\n");
    struct event_base *base = (struct event_base *)ptr;
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
    bufferevent_enable(bev, EV_READ);

}


int main()
{
    
    struct sockaddr_in seraddr;
    bzero(&seraddr, sizeof(seraddr));
    seraddr.sin_family = AF_INET;
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    seraddr.sin_port = htons(8999);

    struct event_base *base = NULL;
    base = event_base_new();

    struct evconnlistener *listener;
    listener = evconnlistener_new_bind(base, cb_listener, base, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE,
                                            36, (struct sockaddr*)&seraddr, sizeof(seraddr));
    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_base_free(base);

    return 0;
}
