#include "unp.h"
#include <pthread.h>
#include "graphics/cui.h"

void* client_console_input(void* args){
	int sockfd=((int*)args)[0];
	char* buf=(char*)malloc(sizeof(char)*BUFF_LEN);

	printf("Console Input thread spawned\n");
	printf("Client action has been called\n");
	memset(buf,'\0',BUFF_LEN);
	while(1){

		char* notEOF=NULL; 	// Used to identify if 
	       				// Ctrl+D has been pressed 
					// by the client. If Ctrl+D 
					// is pressed, notEOF remains
					// NULLL	
		for(int i=0;i<BUFF_LEN && buf[i]!='\0';i++){
			//Reinitialise the buffer with null values
			//to avoid possibility of errors
			//Inefficient step, should find a better way
			buf[i]='\0';
		}
		//printf("Enter your message: \n");
		notEOF=fgets(buf,BUFF_LEN,stdin);

		// If Ctrl+D is pressed, notEOF is NULL
		// and it means the client wants to return 
		if(notEOF==NULL){
			exit(0);
		}
		if(strlen(buf)>1){
			//Ensures that the message has some valid
			//body and is not an empty message

			writen(sockfd,buf,strlen(buf));
		}
	}
}
void* client_console_output(void* args){
	printf("Console Output thread spawned\n");
	int sockfd=((int*)args)[0];
	struct send_recv_info* info=(struct send_recv_info*)malloc(sizeof(struct send_recv_info));
	prepare_screen();
	while(1){
		int b_read=read(sockfd,info,sizeof(struct send_recv_info));
		//printf("messsage in structure: %s\n",info->msg);
		render_msg(info);
		//printf("\n");
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
	if(argc<3){
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
	write(sockfd,argv[1],20);
	client_action(sockfd);
}
