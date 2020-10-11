#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i=0;
    while(i<argc)
    {
        printf("%s\n", argv[i++]);
    }
    return 8;
}