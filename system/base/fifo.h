#ifndef ECL_FIFO_H
#define ECL_FIFO_H

#ifdef ECL_THREAD_ENABLED
#include <pthread.h>
#endif

typedef struct ecl_fifo_entry_s
{
	struct ecl_fifo_entry_s * previous;
	struct ecl_fifo_entry_s * next;
	void * data;
} ecl_fifo_entry_t;

typedef struct
{
	size_t count;
	void * in;
	void * out;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_t mtx;
#endif
} ecl_fifo_t;

int ecl_fifo_init( ecl_fifo_t * fifo );
int ecl_fifo_push( ecl_fifo_t * fifo, void * data );
int ecl_fifo_pop( ecl_fifo_t * fifo, void ** data );
size_t ecl_fifo_count( ecl_fifo_t * fifo );
void ecl_fifo_destroy( ecl_fifo_t * fifo );

#endif
