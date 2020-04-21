#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct node{
	//This is the node that is a part of the queue
	//type has the information regarding the data
	//type of the data stored in node

	char* type;
	void* data;
	struct node* prev;	
};
struct pyqueue{
	struct node *head;
	struct node *tail;
	int len;
};
void pq_init(struct pyqueue* pq){
	//This function can be used to get a new pyqueue
	//Therefore, we can restrict the internal contents 
	//of the pyqueue structure within this file itself.

	pq->head=NULL;
	pq->tail=NULL;
	pq->len=0;
}
static void nd_destroy(struct node* ptr){
	free(ptr);
}
void pq_destroy(struct pyqueue* ptr){
	struct node* temp=ptr->tail;
	while(temp!=NULL){
		struct node* temp2=temp;
		temp=temp->prev;
		nd_destroy(temp);
	}
	free(ptr);
}
struct node* node_init(char* type,void* data){
	struct node* ptr=(struct node*)malloc(sizeof(struct node));
	ptr->type=type;
	ptr->data=data;	
	//printf("New element added: %d\n",*(int*)ptr->data);
	ptr->prev=NULL;
	return ptr;
}
void pq_enqueue(struct pyqueue* pq,char* type,void* elm){
	//printf("New element to be enqueued: %d\n",*((int*)elm));
	struct node* new_node=node_init(type,elm);
	if(pq->len==0){
		pq->head=new_node;
		pq->tail=new_node;
		pq->len++;
		return;
	}
	new_node->prev=pq->tail;
	pq->tail=new_node;
	pq->len++;
	return;
}

void* pq_dequeue(struct pyqueue* pq,char* return_type){
	if(strcmp(return_type,"node")!=0 && strcmp(return_type,"data")!=0){
		fprintf(stderr,"Invalid return type requested\n");
		return NULL;
	}
	if(pq->len==0){
		fprintf(stderr,"Queue size is insufficient\n");
		return NULL;
	}	
	struct node* temp=pq->tail;
	pq->tail=pq->tail->prev;
	pq->len--;
	if(pq->len==0){
		free(pq->head);
		pq->head=NULL;
	}
	if(strcmp(return_type,"node")==0){
		return temp;
	}
	void* data_ptr=temp->data;
	free(temp);
	return data_ptr;
}	

void* pq_peek(struct pyqueue* pq,char* return_type){

	if(strcmp(return_type,"node")!=0 && strcmp(return_type,"data")!=0){
		fprintf(stderr,"Invalid data type\n");
		return NULL;
	}
	if(pq->len==0){
		fprintf(stderr,"Queue size is insufficient\n");
		return NULL;
	}
	if(strcmp(return_type,"node")==0){
		return pq->tail;
	}
	return pq->tail->data;
}

int main(){
	struct pyqueue* pq=(struct pyqueue*)malloc(sizeof(struct pyqueue));
	pq_init(pq);
	int a=100;
	double b=100.13;
	pq_enqueue(pq,"int",&a);
	pq_enqueue(pq,"float",&b);
	double* ret_val=(double*)pq_dequeue(pq,"data");
	int* ret_val2=(int*)pq_dequeue(pq,"data");
	printf("%f\n",*ret_val);
	printf("%d\n",*ret_val2);

	pq_enqueue(pq,"char*","Hello World this is Shayak");
	struct node* nd=(struct node*)pq_dequeue(pq,"node");
	printf("%s\n",(char*)(nd->data));
	pq_enqueue(pq,"int",&a);
	nd=pq_peek(pq,"node");
	printf("%d\n",*(int*)nd->data);

	free(nd);
	free(ret_val);
	free(ret_val2);
	pq_destroy(pq);
}

