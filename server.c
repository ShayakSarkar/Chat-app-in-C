#include <pthread.h>
#include "unp.h"
#include "mylib/pylist.h"
#include <string.h>

#define MAX_CONNECTIONS 5
#define INITIAL

static struct pylist connfds;

void* handle_client(void* args){
	int connfd=((int*)args)[0];
	printf("New Connection fd: %d\n",connfd);
	char* buf=(char*)alloca(sizeof(char)*BUFF_LEN);
	printf("Main server spawned new thread for client\n");
	while(1){
		int b_read=read(connfd,buf,BUFF_LEN);
		printf("Server receieved: %s",buf);
		printf("Bytes read: %d\n",b_read);
		for(int i=0;i<connfds.index;i++){
			int b_written=write(*(int*)pl_get(&connfds,i),buf,b_read);
			printf("Bytes written: %d\n",b_written);
		}
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
