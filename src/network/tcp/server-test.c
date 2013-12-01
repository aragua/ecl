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

#include "server.h"

int net_callback( int sock, struct sockaddr_in * from, void * params )
{
	char buffer[256];
	write( sock, "tcp server\n", 11 );
	read( sock, buffer, 255 );
	printf("%s\n",buffer);
	return 0;
}

int main(int argc, char *argv[])
{
	tcp_server_t ctx;

	ctx.ip=NULL;
	ctx.port=8080;
	ctx.clt_nbr=1;
	ctx.callback=net_callback;
	ctx.prvt_data=NULL;

	if ( ecl_start_tcp_server( &ctx ) < 0 )
	{
		perror("ecl_start_tcp_server");
		return -1;
	}
	
	printf("Succeed\n");	

	return 0;
}

