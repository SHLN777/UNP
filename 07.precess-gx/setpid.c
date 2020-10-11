#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;
    if((pid=fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("zi: %d\n", getpid());
        printf("zipg: %d\n", getpgid(0));
        sleep(5);
        printf("bian: %d\n", getpgid(0));
    }
    sleep(1);

    setpgid(pid, pid);
    sleep(5);

    printf("parent %d\n", getpid());
    printf("parent of parent %d\n", getppid());
    printf("parent %d\n", getpgid(0));

    
}