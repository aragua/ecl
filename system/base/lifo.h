#ifndef ECL_LIFO_H
#define ECL_LIFO_H

#ifdef ECL_THREAD_ENABLED
#include <pthread.h>
#endif

typedef struct ecl_lifo_entry_s
{
	struct ecl_lifo_entry_s * next;
	void * data;
} ecl_lifo_entry_t;

typedef struct
{
	size_t count;
	void * head;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_t mtx;
#endif
} ecl_lifo_t;

int ecl_lifo_init( ecl_lifo_t * lifo, size_t entry_sz );
int ecl_lifo_push( ecl_lifo_t * lifo, void * data );
int ecl_lifo_pop( ecl_lifo_t * lifo, void ** data );
size_t ecl_lifo_count( ecl_lifo_t * lifo );
void ecl_lifo_destroy( ecl_lifo_t * lifo );

#endif
