#include "reader_writer.h"
#include "reader_writer_tracing.h"
//#include "my_queue.h"
#include <stdio.h>

#define READER 0
#define WRITER 1

extern tracing_t t; 

typedef struct my_q_item {
    int thread_id;
    int status;
    struct my_q_item *next;
} my_q_item;

typedef struct my_queue {
  struct my_q_item *head;
  struct my_q_item *tail;
} my_queue;


typedef struct reader_writer{
	int begining;
	pthread_mutex_t mbegining;
	int nb_threads; //tracing t
	pthread_cond_t  cv;
	pthread_cond_t  cv_read_end;
	pthread_mutex_t mqueue;
	my_queue queue;
	int nb_actual_reads;
	int nb_writters_waiting;
	int ids;
	pthread_mutex_t mids;
	pthread_mutex_t mactualreads;
} reader_writer_s; 

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


reader_writer_t rw_init()
{
	//Structure allocation
	reader_writer_t rw = malloc(sizeof(reader_writer_s)); 
	
	rw->begining = 1;
	rw->nb_actual_reads = 0;
	rw->nb_writters_waiting = 0;
	rw->ids = 0;
	
	//Initialisation queue
	init(rw->queue);
	
	//Initialisation mutex
	pthread_mutex_init (&(rw->mbegining), NULL);
	pthread_mutex_init (&(rw->mqueue), NULL);
	pthread_mutex_init (&(rw->mids), NULL);
	pthread_mutex_init (&(rw->mactualreads), NULL);
	
	//Initialisation conditions
	pthread_cond_init (&rw->cv, NULL);
	pthread_cond_init (&rw->cv_read_end, NULL);
	
	return rw; 
}


void begin_read(reader_writer_t rw)
{
	pthread_mutex_lock (&rw->mactualreads);
	rw->ids++;
	int my_id = rw->ids;
	
	my_q_item *item = (my_q_item*)malloc(sizeof(my_q_item));
	item->thread_id = my_id;
	item->status = READER;
	enqueue(rw->queue, item);
	
	if (rw->queue.head == NULL) {
		printf("EROOOOOOOOOOOR\n");
		exit(2);
	}
	
	//nb_actual_reads == -1 if one writter write
	while (rw->nb_actual_reads < 0 || !writter_before(rw->queue, my_id) || rw->begining == 1) {
		pthread_cond_wait (&rw->cv,&rw->mactualreads);
	}
	
	tracing_record_event(t, BR_EVENT_ID);
	
	dequeue(rw->queue);
	rw->nb_actual_reads++;
	
	free(item);

	pthread_mutex_unlock (&rw->mactualreads);
}

void end_read(reader_writer_t rw)
{	
	pthread_mutex_lock (&rw->mactualreads);
	
	rw->nb_actual_reads--;	
	
	if (rw->nb_actual_reads == 0) {
		pthread_cond_signal(&rw->cv);
	}
	
	tracing_record_event(t, ER_EVENT_ID);
	pthread_mutex_unlock (&rw->mactualreads);  

}

void begin_write(reader_writer_t rw)
{
	pthread_mutex_lock(&rw->mactualreads);
	rw->ids++;
	int my_id = rw->ids;
	rw->begining = 0;
	
	my_q_item *item = (my_q_item*)malloc(sizeof(my_q_item));
	item->thread_id = my_id;
	item->status = WRITER;
	enqueue(rw->queue, item);
	while (rw->nb_actual_reads != 0 || rw->queue.head->thread_id == my_id) {
		pthread_cond_wait (&rw->cv,&rw->mactualreads);	
	}
	
	tracing_record_event(t, BW_EVENT_ID);	
	dequeue(rw->queue);
	rw->nb_actual_reads = -1;
	
	free(item);
	
	pthread_mutex_unlock(&rw->mactualreads); 
}

void end_write(reader_writer_t rw){
	pthread_mutex_lock(&rw->mactualreads);
	
	rw->nb_actual_reads = 0;
	tracing_record_event(t, EW_EVENT_ID); 
	
	pthread_cond_signal(&rw->cv);
	
	pthread_mutex_unlock(&rw->mactualreads);  
}

