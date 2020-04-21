#include <stdio.h>
#include <stdlib.h>

void my_free(int* ptr){
	printf("Trying to free: %d\n",*ptr);
	free(ptr);
	printf("Freed successfully: %d\n",*ptr);
}
int main(){
	int* ptr=(int*)malloc(sizeof(int));
	*ptr=100;
	int a=200;
	my_free(ptr);
	my_free(&a);
}

