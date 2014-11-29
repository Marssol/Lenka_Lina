#include "reader_writer.h"
#include "reader_writer_tracing.h"
#include "queue.h"


extern tracing_t t; 

typedef struct reader_writer{
	int begining;
	pthread_mutex_t mbegining;
	int nb_readers;
	int nb_writters;
	pthread_cond_t  *cv_readers;
	pthread_cond_t  *cv_writters;
	pthread_mutex_t mstack;
	Queue_s stack;
	int nb_actual_reads;
	pthread_mutex_t mactualreads;
} reader_writer_s; 

reader_writer_t rw_init(int nb_writters, int nb_readers)
{
	//Structure allocation
	reader_writer_t rw = malloc(sizeof(reader_writer_s)); 
	
	rw->begining = 1;
	rw->nb_readers = nb_readers;
	rw->nb_writters = nb_writters;
	rw->nb_actual_reads = 0;
	
	//Initialisation queue
	stack_nb_init(&(rw->stack), nb_writters, nb_readers);
	
	//Initialisation mutex
	pthread_mutex_init (&(rw->mbegining), NULL);
	pthread_mutex_init (&(rw->mstack), NULL);
	pthread_mutex_init (&(rw->mactualreads), NULL);
	
	//Initialisation conditions
	rw->cv_readers = (pthread_cond_t*)(malloc(nb_readers*sizeof(pthread_cond_t)));
	rw->cv_writters = (pthread_cond_t*)(malloc(nb_writters*sizeof(pthread_cond_t)));
	int i;
    for (i = 0; i < nb_writters; i++) {
        pthread_cond_init (&rw->cv_writters[i], NULL);
    }
	for (i = 0; i < nb_readers; i++) {
        pthread_cond_init (&rw->cv_readers[i], NULL);
    }
	
	return rw; 
}


void begin_read(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, BR_EVENT_ID);

  /* ... */
}

void end_read(reader_writer_t rw){
    /* ... */
  tracing_record_event(t, ER_EVENT_ID);  

    /* ... */
}

void begin_write(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, BW_EVENT_ID);  

  /* ... */
}

void end_write(reader_writer_t rw){
  /* ... */
  tracing_record_event(t, EW_EVENT_ID);  

  /* ... */
}

