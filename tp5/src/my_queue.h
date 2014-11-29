#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <stdlib.h>

#define READER 0
#define WRITER 1

typedef struct my_q_item;

typedef struct my_queue;

void init(my_queue q);
void enqueue(my_queue q, my_q_item* item);
my_q_item* dequeue(my_queue);


#endif // MY_QUEUE_H
