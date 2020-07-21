#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct node{
	void* data;
	char* type;
	struct node* next;
};

static void nd_init(struct node* ptr,void* elm,char* type){
	ptr->data=elm;
	ptr->next=NULL;
	ptr->type=type;
}

struct generic_linked_list{
	struct node* head;
	struct node* tail;
	int len;
};

void gll_init(struct generic_linked_list* ptr){
	ptr->head=NULL;
	ptr->tail=NULL;
	ptr->len=0;
}

void gll_insert(struct generic_linked_list* ptr,void* elm,char*type,int index){
	if(index>ptr->len || index<0){
		fprintf(stderr,"Invalid pointer\n");
		return;
	}
	if(ptr->len==0){
		ptr->head=ptr->tail=(struct node*)malloc(sizeof(struct node));
		nd_init(ptr->head,elm,type);
		ptr->len++;
		return;
	}
	else if(index==0){
		struct node* new_node=(struct node*)malloc(sizeof(struct node));
		nd_init(new_node,elm,type);
		struct node* temp=ptr->head;
		ptr->head=new_node;
		ptr->head->next=temp;
		ptr->len++;
		return;
	}

	struct node* temp=ptr->head;
	for(int i=0;i<index-1;i++){
		temp=temp->next;
	}
	struct node* new_node=(struct node*)malloc(sizeof(struct node));
	nd_init(new_node,elm,type);
	new_node->next=temp->next;
	temp->next=new_node;
	ptr->len++;
}

void gll_delete(struct generic_linked_list* ptr,int index){
	if(ptr->len==0){
		fprintf(stderr,"Not enough elements in the linked list\n");
		return;
	}
	if(index<0 || index>=ptr->len){
		fprintf(stderr,"Invalid Index\n");
		return;
	}
	if(index==0){
		ptr->len--;
		struct node* temp=ptr->head;
		ptr->head=ptr->head->next;
		free(temp);
		return;
	}
	struct node* temp=ptr->head;
	for(int i=0;i<index-1;i++){
		temp=temp->next;
	}
	struct node* temp2=temp->next;
	temp->next=temp->next->next;
	free(temp2);
	ptr->len--;
}

void gll_push(struct generic_linked_list* ptr,void* elm,char*type){
	struct node* new_node=(struct node*)malloc(sizeof(struct node));
	nd_init(new_node,elm,type);
	//printf("Node initialised\n");
	if(ptr->len==0){
		ptr->head=ptr->tail=new_node;
		ptr->len++;
		return;
	}
	ptr->tail->next=new_node;
	ptr->tail=new_node;
	ptr->len++;
	//printf("Successfully inserted\n");
}

void* gll_get(struct generic_linked_list* ptr,int index){
	if(index>=ptr->len || index<0){
		fprintf(stderr,"Index out of bounds\n");
		return NULL;
	}

	struct node* temp=ptr->head;
	for(int i=0;i<index;i++){
		temp=temp->next;
	}
	return temp->data;
}

void gll_destroy(struct generic_linked_list* ptr){
	struct node* index=ptr->head;
	while(index!=NULL){
		struct node* temp=index;
		index=index->next;
		free(temp);
	}
	free(ptr);
}

void gll_display(struct generic_linked_list* ptr){
	struct node* index=ptr->head;
	printf("[ ");
	while(index!=NULL){
		if(strcmp(index->type,"int")==0){
			printf("%d, ",*(int*)index->data);
		}
		else if(strcmp(index->type,"double")==0 || strcmp(index->type,"float")==0){
			printf("%f, ",*(double*)index->data);
		}
		else if(strcmp(index->type,"unsigned int")==0 || strcmp(index->type,"unsigned long")==0){
			printf("%lu, ",*(unsigned long*)index->data);
		}
		else if(strcmp(index->type,"char*")==0){
			printf("%s, ",(char*)index->data);
		}
		else{
			printf("<Object of type: %s>,",index->type);
		}
		index=index->next;
	}
	printf("]\n");

}

/*
int main(){
	printf("\n\n========================= Running Test =====================\n\n");
	struct generic_linked_list* gll=(struct generic_linked_list*)malloc(sizeof(struct generic_linked_list));
	gll_init(gll);
	int a=100;
	double b=200.123;
	char* c="Shayak Sarkar";
	int d=200;
	char* c2="Awesome";
	gll_push(gll,&a,"int");
	gll_push(gll,&b,"double");
	gll_push(gll,c,"char*");
	gll_push(gll,&d,"int");
	gll_push(gll,c2,"char*");
	gll_display(gll);
	printf("\n");

	gll_delete(gll,1);
	gll_display(gll);
	printf("\n");

	gll_delete(gll,0);
	gll_display(gll);
	printf("\n");

	gll_delete(gll,2);
	gll_display(gll);
	printf("\n");

	struct temp_struct{
		int a;
		int b;
	}var;
	gll_insert(gll,&var,"temp_struct",2);
	gll_display(gll);
	printf("\n");

	gll_destroy(gll);

	printf("\n======================== End of Test ==============================\n\n");

}
*/
