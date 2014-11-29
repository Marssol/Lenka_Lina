#include "my_queue.h"

void init(my_queue q)
{
    q.head = NULL;
    q.tail = NULL;
}

void enqueue(my_queue q, my_q_item* item)
{
    if (q.tail != NULL) q.tail->next = item;
    q.tail = item;
    item->next = NULL;
    if (q.head == NULL) {
		q.head = item;
		q.tail = item;
	}
}

my_q_item* dequeue(my_queue q)
{
    if (q.head == NULL) return NULL;
    my_q_item *result = q.head;
    q.head = q.head->next;
    if (q.head==NULL) q.tail = NULL;
    return result;
}

int writter_before(my_queue q, int id)
{
	int bool = 0;
	my_q_item *iterrator = q.head; 
	while (iterrator != NULL) {
		if (iterrator->status == WRITER) {
			return 1;
		}
		if (iterrator->thread_id == id) {
			return 0;
		}
	}
	return -1;
}
