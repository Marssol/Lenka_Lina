#include "queue.h"

typedef struct Identite_s {
	Job my_job;
	int my_id;
}

typedef struct Queue_s {
	Identite_s *me;
	int height;
	int size;
} Queue_s;


void stack_nb_init(Queue_s* stack, int nb_writters, int nb_readers)
{
	stack->size = nb_writters * nb_readers;
	stack->height = 0;
	stack->me = (Identite_s*)malloc(stack->size*sizeof(Identite_s));
}

void stack_nb_push(Queue_s* stack, Job my_job, int my_id)
{
	if (stack->height >= stack->size) {
		stack->me = realloc(stack->me, sizeof(Identite_s) * (stack->size*2));
	}
	stack->me[stack->height].my_job = my_job;
	stack->me[stack->height].my_id = my_id;
	++(stack->height);
}

// prè - condition !(stack->height <= 0)
void stack_nb_pop(Queue_s* stack)
{
	if (stack->height > 0) {
		--(stack->height);
	}
}

void stack_nb_clear(Queue_s* stack)
{
	stack->height = 0;
	free(stack->me);
	stack->me = NULL;
}

// précondition : (stack->height > 0)
int stack_nb_peek(Queue_s* stack)
{
	if (stack->height > O) {
		return stack->me[stack->height - 1];
	}
	return -1;
}
