#ifndef ECL_WAIT_H
#define ECL_WAIT_H

#include <pthread.h>

typedef struct
{	
	pthread_mutex_t mtx;
	pthread_cond_t cond;
} ecl_wait_t;


int ecl_wait_init( ecl_wait_t * ctx );
int ecl_wait_lock( ecl_wait_t * ctx );
int ecl_wait_unlock( ecl_wait_t * ctx );
void ecl_wait_destroy( ecl_wait_t * ctx );

#endif
