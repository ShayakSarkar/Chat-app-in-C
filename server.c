#include <pthread.h>
#include "unp.h"
#include "mylib/pylist.h"
#include "graphics/cui.h"

#define MAX_CONNECTIONS 5
#define INITIAL

static struct pylist connfds;

static void print_struct(struct send_recv_info* ptr){

	//This function is used only for diagonistic purposes
	//This can be used for monitoring what the server is actually
	//sending to the graphics function render_msg(),
	
	printf("Structure info: \n");
	printf("is_sender: %d\n",ptr->is_sender);
	printf("sender_name: %s\n",ptr->sender_name);
	printf("msg: %s\n",ptr->msg);
	printf("msg_len: %d\n",ptr->msg_len);
	printf("\n");

}
void* handle_client(void* args){
	
	//This is the function that is run by each thread spawned by
	//the server for a new client. This function does the following:
	//	
	//	Read the name of the client. The name is supplied by the 
	//	client on the client side only.
	//	
	//	Read the message from the client. It blocks until the 
	//	message is receieved by it
	//
	//		If the message is EOF i.e Ctrl+D, it terminates
	//		the thread
	//
	//	Give the message to all the other active clients after 
	//	wrapping it into a structure containing other relevant 
	//	information using which the graphics unit can display the
	//	message appropriately
	//	
	//


	int connfd=((int*)args)[0];
	char* buf=(char*)alloca(sizeof(char)*BUFF_LEN);
	char* client_name=(char*)alloca(sizeof(char)*20);

	printf("New Connection fd: %d\n",connfd);
	memset(buf,'\0',BUFF_LEN);
	printf("Main server spawned new thread for client\n");
	read(connfd,client_name,20);

	while(1){
		int b_read=read(connfd,buf,BUFF_LEN);
		if(strlen(buf)==0){
			//If Ctrl+D i.e EOF has been pressed,
			//break out of the loop
			
			for(int i=0;i<connfds.index;i++){
				if(*(int*)pl_get(&connfds,i)==connfd){
					pl_delete(&connfds,i);
				}
			}

			break;
		}
		printf("Server receieved: %s",buf);
		printf("Bytes read: %d\n",b_read);
		struct send_recv_info *info;
		for(int i=0;i<connfds.index;i++){
			info=(struct send_recv_info*)malloc(sizeof(struct send_recv_info));

			//Initialise the send_recv_info structure for the graphics unit
			if(connfd==*(int*)pl_get(&connfds,i)){
				info->is_sender=true;
			}
			else{
				info->is_sender=false;
			}
			strcpy(info->msg,buf);
			info->msg[b_read-1]='\0';
			info->msg_len=b_read;
			strcpy(info->sender_name,client_name);
			//Initialisation complete


			printf("Server has pakced %s into the info structure...\n",info->msg);

			print_struct(info);

			//THIS WRITE FUNCTION NEEDS TO CHANGE
			//SHOULD TRANSFER THE send_recv_info STRUCTURE
			int b_written=write(*(int*)pl_get(&connfds,i),info,sizeof(struct send_recv_info));
			free(info);
			printf("Bytes written: %d\n",b_written);
		}
		memset(buf,'\0',b_read);	//Reinitialise the buffer to remove
						//previous entries in the buffer. Should
						//finding an better solution as this step
						//is relevantly inefficient
	}
	return NULL;
}

void accept_incoming(int listenfd){

	//This function is used to accept the incoming socket address 
	//structures from the listening port. After creating a new 
	//connection socket for each of the clients, it spawns a new
	//thread for handling each of the clients. 

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
			//If valid connection is made, we must spawn a 
			//new thread to handle the client

			cli_tid=(pthread_t*)malloc(sizeof(pthread_t)); //Stores Client 
								       //Thread id
			pl_add(&connfds,connfd);	//The connection 
							//descriptor to the 
							//connfds pylist

			pl_add(&tids,cli_tid);	//Add the cli_tid to the pylist connfds
			pthread_create(cli_tid,NULL,handle_client,connfd); //create the thread
		}

	}

	for(int i=0;i<tids.index;i++){

		//Wait for all the threads to join
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
