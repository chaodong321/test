#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<libaio.h>
#include<errno.h>
#include<unistd.h>

int main(int argc, char *argv[]){
    int output_fd;
    char *content="hello world!";
    char *outputfile="hello.txt";
    io_context_t ctx;
    struct iocb io,*p=&io;
    struct io_event e;
    struct timespec timeout;
    memset(&ctx,0,sizeof(ctx));
	
    if(io_setup(10,&ctx)!=0){//init
        printf("io_setup error\n");
        return -1;
    }
	
    if((output_fd=open(outputfile,O_CREAT|O_WRONLY,0644))<0){
        perror("open error");
        io_destroy(ctx);
        return -1;
    }
    io_prep_pwrite(&io,output_fd,(void*)content,strlen(content),0);
    io.data=(void*)content;
    if(io_submit(ctx,1,&p)!=1){
        io_destroy(ctx);
        printf("io_submit error\n");
        return -1;
    }
	
    while(1){
        sleep(1);
        printf("check if done\n");
        timeout.tv_sec=0;
        timeout.tv_nsec=500000000;//0.5s
        if(io_getevents(ctx,0,1,&e,&timeout)==1){
            close(output_fd);
            printf("have done\n");
            break;
        }
        printf("haven't done\n");
    }
    io_destroy(ctx);
    return 0;
}