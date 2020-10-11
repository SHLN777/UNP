#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <event2/event.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void write_cb(evutil_socket_t fd, short what, void *arg)
{
    char buf[1024] = {0};
    static int num = 0;
    sprintf(buf, "hello world-%d\n", num++);
    write(fd, buf, strlen(buf));
    sleep(1);
}

int main()
{
    int fd = open("myfifo", O_WRONLY);

    struct event_base *base = NULL;
    struct event *event = NULL;
    base = event_base_new();
    event = event_new(base, fd, EV_WRITE|EV_PERSIST, write_cb, NULL);

    event_add(event, NULL);
    event_base_dispatch(base);

    event_free(event);
    event_base_free(base); 
    close(fd);


    return 0;
}