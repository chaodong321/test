#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void show_handler(int sig)
{
    printf("I got signal %d\n", sig);
    int i;
    for(i = 0; i < 5; i++) {
        printf("i = %d\n", i);
        sleep(1);
    }
}

int main(void)
{
    int i = 0;
    struct sigaction act, oldact;
    act.sa_handler = show_handler;
    sigaddset(&act.sa_mask, SIGQUIT);
	sigaddset(&act.sa_mask, SIGTERM);
    act.sa_flags = SA_RESETHAND | SA_NODEFER;

    sigaction(SIGINT, &act, &oldact);
    while(1) {
        sleep(1);
        printf("sleeping %d\n", i);
        i++;
    }
}