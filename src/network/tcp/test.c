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

int net_callback( int sock, void * params )
{
	write( sock, "hello\n", 6 );
	return 0;
}

int main(int argc, char *argv[])
{

	if ( ecl_start_tcp_client( "localhost", 8080, net_callback, NULL ) < 0 )
	{
		perror("ecl_start_tcp_client");
		return -1;
	}
	
	printf("Succeed\n");	

	return 0;
}

