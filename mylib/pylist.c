#include <stdio.h>
#include <stdlib.h>

//This is a dynamic list which can be used for storing
//any kind of object just like in python.

#define INIT_LEN 2
struct pylist{
	void** arr;	// Used for storing the actual data pointers
	char** type;	// Used for storing the type information
	int index;
	int len;
};

void pl_init(struct pylist *ptr){
	/*Used to allocate space to the dynamic array
	 */
	ptr->arr=(void**)malloc(sizeof(void*)*INIT_LEN);
	ptr->type=(char**)malloc(sizeof(char*)*INIT_LEN);
	ptr->index=0;
	ptr->len=INIT_LEN;
#undef INIT_LEN
}

static void increase_internal_array(struct pylist* ptr){
	//This function is used to increase the internal array
	//capacity on the overflow of the internal array during
	//insertion or addition of newer elements

	//printf("Internal array size increased due to insufficient memory\n");
	int len=ptr->len;
	len=len*2;	// New size is double of previous size
	void** temparr=ptr->arr;
	char** temparr_char=ptr->type;
	ptr->arr=(void**)malloc(sizeof(void*)*len);
	ptr->type=(char**)malloc(sizeof(char*)*len);
	for(int i=0;i<len/2;i++){
		ptr->arr[i]=temparr[i];
		ptr->type[i]=temparr_char[i];
	}
	ptr->len=len;
	//printf("New size is: %d\n",ptr->len);
	free(temparr);
	free(temparr_char);
}

int pl_add(struct pylist *ptr,char* type,void* elm){
	//Adds a new element to the back of the list

	if(ptr->index >= ptr->len){
		increase_internal_array(ptr);
	}
	ptr->arr[ptr->index]=elm;
	ptr->type[ptr->index]=type;
	ptr->index++;
	//printf("New index: %d\nNew length: %d\n",ptr->index,ptr->len);
	return 0;
}
static void reduce_internal_array(struct pylist* ptr,int index){
	void** temparr=ptr->arr;
	char** temparr_char=ptr->type;
	int len=ptr->len/2;
	ptr->arr=(void**)malloc(sizeof(void*)*len);
	ptr->type=(char**)malloc(sizeof(char*)*len);
	for(int i=0;i<=index;i++){
		ptr->arr[i]=temparr[i];
		ptr->type[i]=temparr_char[i];
	}
	ptr->len=len;
	free(temparr);
	free(temparr_char);
}
int pl_delete(struct pylist *ptr,int index){
	//deletes an element from the list at the index th
	//location. Reduces the size of the array by 1
	if(ptr->index==0){
		fprintf(stderr,"Memory unbderflow\n");
		return -1;
	}
	if(index>=ptr->index || index<0){
		fprintf(stderr,"Index out of bounds\n");
	}
	return -1;
	void** temparr=ptr->arr;
	if(ptr->index-1<(ptr->len)/2){
		printf("Internal array size reduced due to scarcity of elements\n");
		reduce_internal_array(ptr,index-1);	// reduce size of internal array
							// keeping elements upto index-1 constant
	}
	for(int i=index;i<ptr->index-1;i++){
		ptr->arr[i]=temparr[i+1];
	}
	ptr->index--;
	//printf("New index: %d\nNew length: %d\n",ptr->index,ptr->len);
	return 0;
}

int pl_insert(struct pylist* ptr,void* elm,int index){
	//Inserts an element elm, at the index th location in
	//the list

	if(index<0 || index>ptr->index){
		fprintf(stderr,"Index out of bounds\n");
		return -1;
	}
	if(ptr->index>=ptr->len){
		increase_internal_array(ptr);
	}
	void* temp=ptr->arr[index];
	ptr->arr[index]=elm;
	for(int i=index+1;i<ptr->index;i++){
		void* temp2=ptr->arr[i];
		ptr->arr[i]=temp;
		temp=temp2;
	}
	ptr->arr[ptr->index]=temp;
	ptr->index++;
	//printf("New index: %d\nNew length: %d\n",ptr->index,ptr->len);
	return 0;
}

int pl_destroy(struct pylist* ptr){
	//This is basically the destructor for the structure

	free(ptr->arr);
	return 0;
}

void* pl_get(struct pylist* ptr, int index){
	//Returns an element at the index th location in
	//the list

	if(index>=ptr->index || index<0){
		fprintf(stderr,"Index out of bounds\n");
		return NULL;
	}
	return ptr->arr[index];
}
