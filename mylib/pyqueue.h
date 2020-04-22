struct node{
	char*;
	void* data;
	struct node* prev;
};

struct pyqueue{
	struct node *head;
	struct node *tail;
	int len;
};

void pq_init(struct pyqueue*);
void pq_destroy(struct pyqueue*);
void pq_enqueue(struct pyqueue*,char*,void*);
void* pq_dequeue(struct pyqueue*,char*);
void* pq_peek(struct pyqueue*,char*);

