#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *func1(void *arg1)
{
    printf("pthread 1 return");
    return (void *)(9);
}

void *func2(void *arg2)
{
    printf("pthread 2 exit");
    pthread_exit((void *)(10));
}

void *func3(void *arg3)
{
    while(1)
    {
        printf("pthread 3 cancel");
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    void *ret;

    pthread_create(&tid, NULL, func1, NULL);
    pthread_join(tid, &ret);
    printf("%d", (int)ret);

    return 0;
}