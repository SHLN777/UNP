#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char *p = "sss";
    printf("%s\n",p);
    printf("%s\n", "hello");
    execlp("test", "ttest","11","aaa",NULL);
    printf("hellop\n");
}