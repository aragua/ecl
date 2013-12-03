#include "lifo.h"

#include <stdlib.h>

int ecl_lifo_init( ecl_lifo_t * lifo, size_t entry_sz )
{
	if ( !lifo )
		return -1;
		
	lifo->count = 0;
	lifo->head = NULL;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_init( &lifo->mtx, NULL );
#endif
	return 0;
}

int ecl_lifo_push( ecl_lifo_t * lifo, void * data )
{
	if ( !lifo || !data )
		return -1;
	
	ecl_lifo_entry_t * entry = malloc(sizeof(ecl_lifo_entry_t));
	if ( !entry )
		return -1;

	entry->data = data;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &lifo->mtx );
#endif
	entry->next = lifo->head;
	lifo->head = entry;
	lifo->count++;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &lifo->mtx );
#endif
	
	return 0;
}

int ecl_lifo_pop( ecl_lifo_t * lifo, void ** data )
{
	ecl_lifo_entry_t * tmp;

	if ( !lifo || !data )
		return -1;

#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &lifo->mtx );
#endif
	tmp = lifo->head;
	if ( lifo->count > 0 && tmp )
	{
		*data = tmp->data;
		lifo->head = tmp->next;
		lifo->count--;
	}
	else
		*data = NULL;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &lifo->mtx );
#endif
	if ( tmp )
		free(tmp);
	
	return 0;
}

size_t ecl_lifo_count( ecl_lifo_t * lifo )
{
	size_t ret = 0;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_lock( &lifo->mtx );
#endif
	ret = lifo ? lifo->count : -1;
#ifdef ECL_THREAD_ENABLED
	pthread_mutex_unlock( &lifo->mtx );
#endif
	return ret;
}
	
void ecl_lifo_destroy( ecl_lifo_t * lifo )
{
	if ( lifo )
	{
		int idx, until = ecl_lifo_count(lifo);
		void * tmp;
		for ( idx = 0 ; idx < until ; idx++ )
		{
			ecl_lifo_pop(lifo, &tmp);
		}
#ifdef ECL_THREAD_ENABLED
		pthread_mutex_destroy( &lifo->mtx );
#endif
	}
}

