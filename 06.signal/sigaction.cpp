#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

void capture(int a)
{
    cout<<"tuichu"<<endl;
}

int main()
{
    struct  sigaction  act;

    act.sa_handler = capture;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;

    sigaction(SIGINT, &act, NULL);

    while(1)
    {
        cout<<"*********"<<endl;
        sleep(1);
    }
   
    return 0;
}