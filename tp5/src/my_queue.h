#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <stdlib.h>

#define READER 0
#define WRITER 1

struct _q_item;
struct _q_item
{
    int thread_id;
    int status;
    struct _q_item* next;
};
typedef struct _q_item q_item;
typedef struct _queue
{
  q_item* head;
  q_item* tail;
} my_queue;

void init(my_queue q);
void enqueue(my_queue q, q_item* item);
q_item* dequeue(my_queue);


#endif // MY_QUEUE_H
