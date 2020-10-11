#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
  int fd[2];
  pid_t pid;
  char buf1[1024] = "hello hello";
  char buf2[1024];
  pid = fork();
  if(pid < 0)
  {
    perror("pid");
    exit(1);
  }
  if(pid > 0)
  {
   if(pipe(fd)<0)
      perror("pipe");
      exit(1);
   else
   {
      close(fd[0]);
      write(fd[1], buf1, strlen(buf1));
   }
  }

}
