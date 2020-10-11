#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd, i=0;
    char *P;
    off_t pos;
    if(argc < 2)
    {
        printf("canshu");
        exit(1);
    }
    fd = open(argv[1], O_RDWR);
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
        perror("mmap");
        exit(1);
    }
    close(fd);
    while(1)
    {
        printf("%s\n", P);
        sleep(1);
    }
    munmap(P, 0x1000);

    return 0;
}
