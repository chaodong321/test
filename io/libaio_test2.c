#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <libaio.h>
#include <errno.h>
#include <unistd.h>
#include <unistd.h>

#define MAX_COUNT 10 * 1024
#define BUF_SIZE  1 * 1024 * 1024

#ifndef O_DIRECT
#define O_DIRECT         040000 /* direct disk access hint */
#endif

int main(int args, void *argv[]){
    int fd;
    void * buf = NULL;

    int pagesize = sysconf(_SC_PAGESIZE);
    posix_memalign(&buf, pagesize, BUF_SIZE);

    io_context_t ctx;
    struct iocb io,*p=&io;
    struct io_event e[10];
    struct timespec timeout;

    memset(&ctx,0,sizeof(ctx));
    if(io_setup(10,&ctx)!=0){
        printf("io_setup error\n");
        return -1;
    }

    if((fd = open("test.log", O_WRONLY | O_CREAT | O_APPEND | O_DIRECT, 0644))<0) {
        perror("open error");
        io_destroy(ctx);
        return -1;
    }

    int n = MAX_COUNT;

    while(n > 0) {
        io_prep_pwrite(&io, fd, buf, BUF_SIZE, 0);

        if(io_submit(ctx, 1, &p)!=1) {
            io_destroy(ctx);
            printf("io_submit error\n");
            return -1;
        }

        int ret = io_getevents(ctx, 1, 10, e, NULL);
        if (ret != 1) {
            perror("ret != 1");
            break;
        }
        n--;
    }

    close(fd);
    io_destroy(ctx);
    return 0;
}