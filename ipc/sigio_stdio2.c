#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

static int cnt = 0;
void sigio_handler(int sig)
{
    char buf[100];
    cnt++;
    read(STDIN_FILENO, buf, 100);
}

int main(int argc, char **argv)
{
    struct sigaction sig_act;
    int fl = 0;
    setbuf(stdin, NULL);
    setbuf(stdout,NULL);
    fl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK | O_ASYNC ); //设置O_ASYNC后成为异步IO，不设置该值则为信号驱动IO
    fcntl(STDIN_FILENO, F_SETOWN, getpid());  //设置接收SIGIO的进程
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = sigio_handler;  //设置信号接收函数
    sigaction(SIGIO, &sig_act, NULL);    //注册SIGIO信号

    time_t st=time(NULL);
    while (time(NULL) < st+5)
    {
        sleep(1);
    }
    printf("cnt=%d\n", cnt);

    return 0;
}
