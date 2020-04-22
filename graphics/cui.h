#include <stdbool.h>
struct send_recv_info{
	bool is_sender;
	char sender_name[20];
	char msg[1000];
	int msg_len;
};
void prepare_screen();
void render_msg(struct send_recv_info*);
