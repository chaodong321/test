/**
 STDIN_FILENO：接收键盘的输入
 STDOUT_FILENO：向屏幕输出
 
 fflush()会强迫将缓冲区内的数据写回参数stream 指定的文件中。
 fflush(stdin)刷新标准输入缓冲区，把输入缓冲区里的东西丢弃[非标准]
 fflush(stdout)刷新标准输出缓冲区，把输出缓冲区里的东西打印到标准输出设备上
 
 信号驱动IO设置步骤：
 使用fcntl来置O_ASYNC位。
   这个方法的效果是，当输入缓存中的输入数据就绪时（输入数据可读），内核向用F_SETOWN来绑定的那个进程发送SIGIO信号。此时程序应该用getchar等函数将输入读入。
   1.首先，为SIGIO信号设置一个处理函数，用来读取并处理位于输入缓存中的数据。
			  signal ( SIGIO , void ( * getmyinput ) ( int signum ) );

   2.设置一个用来接受SIGIO信号的进程。用fcntl函数。
			  fcntl ( my_fd , F_SETOWN, getpid() );

   3.得到文件描述符的状态标志集，为该状态标志集添加一个O_ASYNC属性。
			  int  flags = fcntl ( my_fd , F_GETFL);
			  fcntl ( my_fd , F_SETFL , flags | O_ASYNC);
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>


void sigio_handler(int sig)
{
    static int cnt = 0;

    printf("receive SIGIO signal %d\n", ++cnt);
    fflush(stdin);
    fflush(stdout);
}

int main(int argc, char **argv)
{
    struct sigaction sig_act;
    int fl = 0;

    //signal driven I/O setting
    fflush(stdin);
    fflush(stdout);
    fl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, fl | O_NONBLOCK | O_ASYNC );
    fcntl(STDIN_FILENO, F_SETOWN, getpid());
    sigemptyset(&sig_act.sa_mask);
    sig_act.sa_flags = 0;
    sig_act.sa_handler = sigio_handler;
    sigaction(SIGIO, &sig_act, NULL);

    printf("test start\n");
    fflush(stdin);
    fflush(stdout);

    while (1)
    {
        sleep(1);
    }

    return 0;
}
