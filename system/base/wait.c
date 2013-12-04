#include "wait.h"

int ecl_wait_init( ecl_wait_t * ctx )
{
	if ( !ctx )
		return -1;

	pthread_mutex_init( &ctx->mtx, NULL );
	pthread_cond_init( &ctx->cond, NULL );
	return 0;
}

int ecl_wait_lock( ecl_wait_t * ctx )
{
	pthread_mutex_lock(&ctx->mtx);
	pthread_cond_wait(&ctx->cond, &ctx->mtx);
	pthread_mutex_unlock(&ctx->mtx);
	return 0;
}

int ecl_wait_unlock( ecl_wait_t * ctx )
{
        pthread_mutex_lock(&ctx->mtx);
        pthread_cond_signal(&ctx->cond);
        pthread_mutex_unlock(&ctx->mtx);
        return 0;
}

void ecl_wait_destroy( ecl_wait_t * ctx )
{
	if ( ctx )
	{
		pthread_mutex_init( &ctx->mtx, NULL );
		pthread_cond_init( &ctx->cond, NULL );
	}
}


