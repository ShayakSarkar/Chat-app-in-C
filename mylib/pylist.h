struct pylist{
	void** arr;
	int index;
	int len;
};
void pl_init(struct pylist *ptr);
int pl_add(struct pylist*,void*);
int pl_delete(struct pylist*,int);
int pl_insert(struct pylist*,void*,int);
int pl_destroy(struct pylist* ptr);
void* pl_get(struct pylist*, int);

