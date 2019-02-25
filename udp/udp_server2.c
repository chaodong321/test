#include<sys/socket.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define MAXLINE 80
#define SERV_PORT 8888
 
void do_echo(int sockfd,struct sockaddr *pcliaddr,socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];
	for(;;)
	{
		len=clilen;
		/*waiting for receive data*/
		n=recvfrom(sockfd,mesg,MAXLINE,0,pcliaddr,&len);
		/*sent data back to client*/
		sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);/*create a socket*/
	/*init serv addr*/
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);
	
	/*bind address and port to socket*/
	if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))==-1)
	{
		perror("bind error");
		exit(1);
	}
	do_echo(sockfd,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
	
	return 0;
}