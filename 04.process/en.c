#include <stdio.h>

int main(void)
{
    char a[] = "asdadad";
    char *r = a;
    const char *p = "abc";
    char w = 'a';
    char *c = &w;
    printf("%s", c);

    return 0;
}