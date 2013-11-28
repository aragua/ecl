#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include "client.h"

int ecl_start_tcp_client( char * ip, short port, tcp_client_callback callback, void * params )
{	
	struct sockaddr_in addr;
	int sock, ret;
	
	if ( !ip || port < 0 || !callback )
	{
		fprintf( stderr, "Invalid parameter\n" );
		return -1;
	}
	
	
	memset( &addr, 0, sizeof(struct sockaddr_in) );
	
	addr.sin_addr.s_addr = inet_addr(ip);
	if( addr.sin_addr.s_addr == (unsigned long)INADDR_NONE)
	{
		struct hostent *hostp;
		
		hostp = gethostbyname(ip);
		if( hostp == (struct hostent *)NULL )
		{
			fprintf( stderr, "Bad address %s\n", ip );
			return -1;
		}
		memcpy( &addr.sin_addr, hostp->h_addr, sizeof(addr.sin_addr));
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	
	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sock < 0 )
	{
		fprintf( stderr, "Socket failed %s\n", strerror(errno) );
		return -1;
	}
	
	if( connect( sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("connect error");
		close(sock);
		return -1;
	}
	
	ret = callback( sock, params );
	
	close(sock);

	return ret;
}
