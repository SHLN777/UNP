#include <signal.h>
#include <stdio.h>

void pringsigset(const sigset_t *set)
{
  int i;
  for(i=1; i<32; i++)
  {
    if(sigismember(set, i) == 1)
        //putchar('1');
        printf("%d", 1);
        
    else
        //putchar('0');
        printf("%d", 0); 
       
  }
  puts(" ");

}

int main(void)
{

  sigset_t s, p;
  sigemptyset(&s);
  sigaddset(&s, SIGINT);
  sigprocmask(SIG_BLOCK, &s, NULL);
  while(1)
  {
    sigpending(&p);
    pringsigset(&p);
    sleep(1);
  }

  return 0;
}
