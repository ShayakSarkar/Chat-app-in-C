#include "unp.h"
#include <pthread.h>

void* client_console_input(void* args){
	printf("Console Input thread spawned\n");
	int sockfd=((int*)args)[0];
	printf("Client action has been called\n");
	char* buf=(char*)malloc(sizeof(char)*BUFF_LEN);
	while(1){
		printf("Enter your message: \n");
		fgets(buf,BUFF_LEN,stdin);
		writen(sockfd,buf,strlen(buf));
	}
}
void* client_console_output(void* args){
	printf("Console Output thread spawned\n");
	int sockfd=((int*)args)[0];
	char* buf=(char*)malloc(sizeof(char)*BUFF_LEN);
	while(1){
		int b_read=read(sockfd,buf,BUFF_LEN);
		fwrite(buf,1,b_read,stdout);
		printf("\n");
	}
}
void client_action(int sockfd){
	pthread_t inp_tid,op_tid;
	pthread_create(&inp_tid,NULL,client_console_input,&sockfd);
	pthread_create(&op_tid,NULL,client_console_output,&sockfd);

	pthread_join(inp_tid,NULL);
	pthread_join(op_tid,NULL);
}
int main(int argc,char** argv){
	struct sockaddr_in servaddr;
	const char* ipaddr=NULL;
	if(argc<2){
		ipaddr="127.0.0.1";
	}
	else{
		ipaddr=argv[1];
	}

	printf("IP ADDRESS: %s\n",ipaddr);

	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	
	servaddr.sin_family=AF_INET;
	inet_pton(AF_INET,ipaddr,&servaddr.sin_addr);
	servaddr.sin_port=htons(SERV_PORT1);

	printf("echo 1\n");
	connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	
	if(errno!=0){
		fprintf(stderr,"Error in establishing connection\n");
		perror(NULL);
		return errno;
	}

	client_action(sockfd);
}
