#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd, *P, i=0;
    off_t pos;
    if(argc < 2)
    {
        printf("canshu");
        exit(1);
    }
    fd = open(argv[1], O_RDONLY | O_CREAT, 0777);
    if(fd < 0)
    {
        perror("fd");
    }
    if(lseek(fd, 0x1000-1, SEEK_END) < 0)
    {
        perror("lseek");
        exit(1);
    }
    write(fd, "\0", 1);
   
    P = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(P == MAP_FAILED)
    {
        perror("mmapmap");
        exit(1);
    }
    close(fd);
    while(1)
    {
        sprintf(P, "hello%d", i++);
        sleep(1);
    }
    munmap(P, 0x1000);

    return 0;
}