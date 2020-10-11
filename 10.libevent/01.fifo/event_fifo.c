#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <event2/event.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



void read_cb(evutil_socket_t fd, short what, void *arg)
{
    int n;
    char buf[1024];
    if((n=read(fd,buf,sizeof(buf))) >0)
    {
        printf("read event: %s \n", what & EV_READ?"yes":"no");
        printf("read len=%d, buf=%s\n", n, buf);
    }
}

int main(void)
{

    mkfifo("myfifo", 0664);
    int fd=open("myfifo", O_RDWR, 0664);
    struct event_base *base = NULL;
    base = event_base_new();
    struct event *event;
    event = event_new(base, fd, EV_READ|EV_PERSIST, read_cb, NULL);

    event_add(event, NULL);
    event_base_dispatch(base);

    event_free(event);
    event_base_free(base);
    close(fd);


    return 0;
}

