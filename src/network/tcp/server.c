#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

int ecl_start_tcp_server( tcp_server_t * ctx )
{
	if ( !ctx )
	{
		return -1;
	}

	ctx->sock = socket(AF_INET, SOCK_STREAM, 0);
	if ( ctx->sock < 0) 
	{
		perror("ERROR opening socket");
		return -1;
	}

	/* Initialize socket structure */
	memset( &ctx->addr, 0, sizeof(struct sockaddr_in));
	ctx->addr.sin_family = AF_INET;
	if ( !ctx->ip )
		ctx->addr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		ctx->addr.sin_addr.s_addr = inet_addr(ctx->ip);
		if( ctx->addr.sin_addr.s_addr == (unsigned long)INADDR_NONE)
			ctx->addr.sin_addr.s_addr = INADDR_ANY;
	}
	ctx->addr.sin_port = htons(ctx->port);

	/* Now bind the host address using bind() call.*/
	if ( bind( ctx->sock, (struct sockaddr *) &ctx->addr, sizeof(struct sockaddr_in)) < 0)
	{
        	perror("ERROR on binding");
        	return -1;
	}

	/* Now start listening for the clients, here 
	 * process will go in sleep mode and will wait 
	 * for the incoming connection
	 */
	if ( listen( ctx->sock, ctx->clt_nbr) < 0 )
	{
		return -1;
	}
	while ( !ctx->stop )
	{
		int newsock, ret;
		socklen_t fromlen = sizeof(struct sockaddr_in);
		struct sockaddr_in from;
	
		newsock = accept( ctx->sock, (struct sockaddr *) &from, &fromlen);
		if ( newsock < 0)
		{
			perror("ERROR on accept");
			return -1;
		}

#ifdef ECL_THREADING_ENABLED
		return ret;
#else
		ret = ctx->callback( newsock, &from, ctx->prvt_data );
		close(newsock);
		return ret;
#endif
	}
	return 0;
}

