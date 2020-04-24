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

	//destroy the node
	free(ptr);
}
void pq_destroy(struct pyqueue* ptr){
	
	//Destructor for the pyqueue
	struct node* temp=ptr->tail;
	while(temp!=NULL){
		struct node* temp2=temp;
		temp=temp->prev;
		nd_destroy(temp);
	}
	free(ptr);
}
static struct node* node_init(char* type,void* data){

	//Constructor for the node. It returns a new node
	//setting the internal data of the node as per the 
	//parameters that have been provided

	struct node* ptr=(struct node*)malloc(sizeof(struct node));
	ptr->type=type;
	ptr->data=data;	
	//printf("New element added: %d\n",*(int*)ptr->data);
	ptr->prev=NULL;
	return ptr;
}
void pq_enqueue(struct pyqueue* pq,char* type,void* elm){
	
	//Adds an element at the end of the queue. pq is the pointer 
	//to the queue to which the element must be added.
	//type is the data type of the supplied element. This is added 
	//to allow for easy deferencing by the user later on.
	//elm is the pointer to the actual data that is to be stored 
	//in the pyqueue
	
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

	//Removes an element from the front of the queue. 
	//return_type specifies whether the data returned by
	//the queue is a pointer to the node or the data 
	//within the node. return_type is either 'data' or 
	//'node'.

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

	//Returns the element at the front of the queue
	//This can be either the node, or the data contained
	//within the node. return_type is either 'node' or 'data'

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

/*
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
*/
