#include <string.h>
#include <stdio.h>
#include "hash.h"

#define BREAK_POINT 0.5
struct datum{
	char** type;
	struct generic_linked_list* ptr;
};
static void datum_init(struct datum* ptr,char** type,void* elm){
	ptr->type=type;
	ptr->data=elm;
}
struct pyset{
	struct datum* arr;	//Array of data elements
	int len;
	int count;	// Stores the number of elements currently present	
	double load_factor;	
};
void ps_init(struct pyset* ptr,int init_size){
	ptr->arr=(struct datum*)malloc(sizeof(struct datum));
	ptr->len=0;
	ptr->load_factor=0;
}

static void reallocate(struct pyset *ptr){

}

void ps_add(struct pyset* ptr,void* elm){
	if(ptr->load_factor<BREAK_POINT){
		int hash_val=hash(elm,ptr->len);
		ptr->arr[hash_val]=elm;
		ptr->count++;
		ptr->load_factor=ptr->count/ptr->load_factor;
		return;
	}
	ps_reallocate(ptr);
	int hash_val=hash(elm,ptr->len);
	ptr->arr[hash_val]=elm;
	ptr->count++;
	ptr->load_factor=ptr->count/ptr->load_factor;
	return;
}


