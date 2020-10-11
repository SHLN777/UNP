#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


void *func(void *arg)
{
    int *p = (int *)arg;
    printf("ztid = %x\n",(unsigned int)pthread_self());
    printf("pid = %d\n",getpid());
    printf("t = %d\n", *p);
    
}

int main()
{
    pthread_t tid;
    int t =10;
    pthread_create(&tid, NULL, func, (void*)&t);
    sleep(1);
    printf("zhxch = %x\n", (unsigned int)pthread_self());
    printf("pid = %d\n", getpid());
    printf("ztid = %x\n", (unsigned int)tid);


    return 0;
}