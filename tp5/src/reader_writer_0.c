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

reader_writer_t rw_init(){
  reader_writer_t rw = malloc(sizeof(reader_writer_s)); 

  /* ... */
  
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

