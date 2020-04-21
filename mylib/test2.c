#include <stdio.h>
#include "pylist.h"

void print_array(struct pylist* ptr){
	printf("List elements are: \n");
	for(int i=0;i<ptr->index;i++){
		printf(" %d\n",*(int*)pl_get(ptr,i));
	}
	printf("\n");
}
int main(){
	struct pylist pl;
	pl_init(&pl);
	int a=0;
	int b=1;
	int c=2;
	int d=3;

	pl_add(&pl,&a);
	pl_add(&pl,&b);
	pl_add(&pl,&c);
	pl_add(&pl,&d);
	
	int elm=4;

	pl_insert(&pl,&elm,0);
	
	printf("\n\n");
	print_array(&pl);

	pl_delete(&pl,2);

	print_array(&pl);
	printf("\n");
	
	pl_delete(&pl,2);
	print_array(&pl);
	printf("\n");

	pl_delete(&pl,2);
	print_array(&pl);
	printf("\n");

	pl_delete(&pl,0);
	print_array(&pl);
	printf("\n");

	pl_destroy(&pl);
}
