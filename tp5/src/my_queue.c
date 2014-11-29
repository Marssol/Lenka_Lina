#include "my_queue.h"

typedef struct my_q_item
{
    int thread_id;
    int status;
    struct my_q_item *next;
};

typedef struct my_queue
{
  struct my_q_item *head;
  struct my_q_item *tail;
} my_queue;

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
