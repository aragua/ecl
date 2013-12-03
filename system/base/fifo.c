#include "fifo.h"

#include <stdlib.h>

int ecl_fifo_init( ecl_fifo_t * fifo, size_t entry_sz )
{
	if ( !fifo )
		return -1;
		
	fifo->count = 0;
	fifo->in = NULL;
	fifo->out = NULL;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_init( &fifo->mtx, NULL );
#endif
	return 0;
}

int ecl_fifo_push( ecl_fifo_t * fifo, void * data )
{
	if ( !fifo || !data )
		return -1;
	
	ecl_fifo_entry_t * entry = malloc(sizeof(ecl_fifo_entry_t));
	if ( !entry )
		return -1;

	entry->previous = NULL;
	entry->data = data;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &fifo->mtx );
#endif
	if ( fifo->in )
		((ecl_fifo_entry_t *)fifo->in)->previous = entry;
	else
		fifo->out = entry;
	entry->next = fifo->in;
	fifo->in = entry;
	fifo->count++;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &fifo->mtx );
#endif
	
	return 0;
}

int ecl_fifo_pop( ecl_fifo_t * fifo, void ** data )
{
	ecl_fifo_entry_t * tmp;

	if ( !fifo || !data )
		return -1;

#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &fifo->mtx );
#endif
	tmp = fifo->out;
	if ( fifo->count > 0 && tmp )
	{
		*data = tmp->data;
		fifo->out = tmp->previous;
		if ( !fifo->out )
			fifo->in = NULL;
		fifo->count--;
	}
	else
		*data = NULL;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &fifo->mtx );
#endif
	if ( tmp )
		free(tmp);
	
	return 0;
}

size_t ecl_fifo_count( ecl_fifo_t * fifo )
{
	size_t ret = 0;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &fifo->mtx );
#endif
	ret = fifo ? fifo->count : -1;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &fifo->mtx );
#endif
	return ret;
}
	
void ecl_fifo_destroy( ecl_fifo_t * fifo )
{
	if ( fifo )
	{
		int idx, until = ecl_fifo_count(fifo);
		void * tmp;
		for ( idx = 0 ; idx < until ; idx++ )
		{
			ecl_fifo_pop(fifo, &tmp);
		}
#ifdef ECL_THREAD_ENABLED
		pthread_mutex_destroy( &fifo->mtx );
#endif
	}
}

