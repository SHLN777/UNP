#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  int fd;
  
  fd = open("abc", O_CREAT, 0777);
  printf("fd = %d\n", fd);

  return 0;
}
