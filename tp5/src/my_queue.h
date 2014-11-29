#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <stdlib.h>

#define READER 0
#define WRITER 1

typedef struct my_q_item {
    int thread_id;
    int status;
    struct my_q_item *next;
} my_q_item;

typedef struct my_queue {
  struct my_q_item *head;
  struct my_q_item *tail;
} my_queue;

void init(my_queue q);
void enqueue(my_queue q, my_q_item* item);
my_q_item* dequeue(my_queue);
int writter_before(my_queue q, int id);


#endif // MY_QUEUE_H
