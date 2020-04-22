#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct send_recv_info{
	bool is_sender;
	char sender_name[20];
	char msg[1000];
	int msg_len;
};
void prepare_screen(){
	for(int i=0;i<100;i++){
		printf("\n");
	}
}
static void render_right(char* msg,int msg_len,char* sender_name){
#define PADDING 5
	for(int i=0;i<(80-(PADDING*2)-msg_len);i++){
		printf(" ");
	}
	printf("  %s\n",sender_name);
	for(int i=0;i<(80-(PADDING*2)-msg_len);i++){
		printf(" ");
	}
	printf(" ");
	for(int i=0;i<msg_len+PADDING*2;i++){
		printf("=");
	}
	printf("\n");
	for(int i=0;i<(80-(PADDING*2)-msg_len);i++){
		printf(" ");
	}
	printf("|");
	for(int i=0;i<PADDING;i++){
		printf(" ");
	}
	printf("%s",msg);
	for(int i=0;i<PADDING;i++){
		printf(" ");
	}
	printf("|");
	printf("\n");

	for(int i=0;i<(80-(PADDING*2)-msg_len);i++){
		printf(" ");
	}
	printf(" ");
	for(int i=0;i<msg_len+PADDING*2;i++){
		printf("=");
	}
	printf("\n\n");
}
static void render_left(char* msg,int msg_len,char* sender_name){
	printf("  %s\n",sender_name);
	printf(" ");
	for(int i=0;i<msg_len+PADDING*2;i++){
		printf("=");
	}
	printf("\n");
	printf("|");
	for(int i=0;i<PADDING;i++){
		printf(" ");
	}	
	printf("%s",msg);
	for(int i=0;i<PADDING;i++){
		printf(" ");
	}
	printf("|\n");
	printf(" ");
	for(int i=0;i<msg_len+PADDING*2;i++){
		printf("=");
	}
	printf("\n\n");

}
void render_msg(struct send_recv_info* info){
	if(info->is_sender){
		render_right(info->msg,info->msg_len,info->sender_name);
		return;
	}
	render_left(info->msg,info->msg_len,info->sender_name);	
}
/*
int main(){
	prepare_screen();
	struct send_recv_info info;
	strcpy(info.msg,"This is the message.");
	strcpy(info.sender_name,"SenderName");
	info.is_sender=false;
	info.msg_len=strlen(info.msg);
	render_msg(&info);
}*/
