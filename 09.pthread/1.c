#include <stdio.h>

int main()
{
    int *p = (void *)(10);
    printf("%d", *p);
    return 0;
}