#include "my_queue.h"

void init(my_queue q)
{
    q.head = NULL;
    q.tail = NULL;
}

void enqueue(my_queue q, q_item* item)
{
    if (q.tail!=NULL) q.tail->next = item;
    q.tail = item;
    item->next = NULL;
    if (q.head==NULL) q.head = item;
}

q_item* dequeue(my_queue q)
{
    if(q.head==NULL) return NULL;
    q_item* result = q.head;
    q.head = q.head->next;
    if (q.head==NULL) q.tail = NULL;
    return result;
}
