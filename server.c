#include <pthread.h>
#include "unp.h"
#include "mylib/pylist.h"
#include "graphics/cui.h"

#define MAX_CONNECTIONS 5
#define INITIAL

static struct pylist connfds;

void print_struct(struct send_recv_info* ptr){
	printf("Structure info: \n");
	printf("is_sender: %d\n",ptr->is_sender);
	printf("sender_name: %s\n",ptr->sender_name);
	printf("msg: %s\n",ptr->msg);
	printf("msg_len: %d\n",ptr->msg_len);
	printf("\n");

}
void* handle_client(void* args){
	int connfd=((int*)args)[0];
	printf("New Connection fd: %d\n",connfd);
	char* buf=(char*)alloca(sizeof(char)*BUFF_LEN);
	memset(buf,'\0',BUFF_LEN);
	printf("Main server spawned new thread for client\n");
	char* client_name=(char*)alloca(sizeof(char)*20);
	read(connfd,client_name,20);

	while(1){
		int b_read=read(connfd,buf,BUFF_LEN);
		printf("Server receieved: %s",buf);
		printf("Bytes read: %d\n",b_read);
		struct send_recv_info *info;
		for(int i=0;i<connfds.index;i++){
			info=(struct send_recv_info*)malloc(sizeof(struct send_recv_info));
			if(connfd==*(int*)pl_get(&connfds,i)){
				info->is_sender=true;
			}
			else{
				info->is_sender=false;
			}
			strcpy(info->msg,buf);
			info->msg[b_read-1]='\0';
			printf("Server has pakced %s into the info structure...\n",info->msg);
			info->msg_len=b_read;
			strcpy(info->sender_name,client_name);

			print_struct(info);

			//THIS WRITE FUNCTION NEEDS TO CHANGE
			//SHOULD TRANSFER THE send_recv_info STRUCTURE
			int b_written=write(*(int*)pl_get(&connfds,i),info,sizeof(struct send_recv_info));
			free(info);
			printf("Bytes written: %d\n",b_written);
		}
		memset(buf,'\0',b_read);
	}
	return NULL;
}

void accept_incoming(int listenfd){
	struct sockaddr_in cliaddr;
	int* connfd;
	pthread_t* cli_tid;	// Address of the thread id of the newly created client
	struct pylist tids;	// Stores all thread ids spawned
	pl_init(&tids);
	while(1){
		connfd=(int*)malloc(sizeof(int));
		socklen_t len=sizeof(cliaddr);
		*connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
		if(errno!=0){
			perror("Error in accepting client sockets");
			return;
		}
		else{
			pl_add(&connfds,connfd);
			cli_tid=(pthread_t*)malloc(sizeof(pthread_t));
			pl_add(&tids,cli_tid);
			pthread_create(cli_tid,NULL,handle_client,connfd);
		}

	}
	for(int i=0;i<tids.index;i++){
		pthread_join(*(int*)pl_get(&tids,i),NULL);
	}
	
}
int main(){
	pl_init(&connfds);
	int listenfd;
	struct sockaddr_in servaddr;
	
	listenfd=socket(AF_INET,SOCK_STREAM,0);
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(SERV_PORT1);
	servaddr.sin_addr.s_addr=INADDR_ANY;

	bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(errno){
		perror("bind() error");
		return errno;
	}

	listen(listenfd,5);
	if(errno!=0){
		fprintf(stderr,"Error in listening\n");
		perror(NULL);
	}
	printf("Main server is listening\n");
	
	accept_incoming(listenfd);
}
