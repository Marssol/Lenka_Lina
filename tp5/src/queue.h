#ifndef   	_QUEUE_H_
#define   	_QUEUE_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct Queue_s;
typedef enum {W, R} Job;
typedef struct Identite_s;

void pile_nb_init(Queue_s* stack, int nb_writters, int nb_readers);	
void pile_nb_push(Queue_s* stack, Job my_job, int my_id);
void pile_nb_pop(Queue_s* stack);
void pile_nb_clear(Queue_s* stack);
int pile_nb_peek(Queue_s* stack);	// Retourne la valeur du sommet

#endif
