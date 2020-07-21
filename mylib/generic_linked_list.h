struct node{
	void* data;
	char* type;
	struct node* next;
};
struct generic_linked_list{
	struct node* head;
	struct node* tail;
	int len;
};

void gll_init(struct generic_linked_list*);
void gll_insert(struct generic_linked_list*,void*,char*,int);
void gll_delete(struct generic_linked_list*,int);
void gll_push(struct generic_linked_list*,void*,char*);
void* gll_get(struct generic_linked_list*,int);
void gll_destroy(struct generic_linked_list*);
void gll_destroy(struct generic_linked_list*);
void gll_display(struct generic_linked_list*);

