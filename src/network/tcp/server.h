#ifndef SRC_NETWORK_TCP_SERVER_H
#define SRC_NETWORK_TCP_SERVER_H

#include <netinet/in.h>

typedef struct
{
	/* parameters */
	char * ip;
	short port;
	unsigned int clt_nbr; 
	int (*callback)( int sock, struct sockaddr_in * from, void * params );
	void * prvt_data;
	/* tcp_server private data */
	int sock;
	int stop;
	struct sockaddr_in addr;
} tcp_server_t;

/**
  * This function create a client and call a function to use this socket.
  * then it closes the socket
  */
int ecl_start_tcp_server( tcp_server_t * ctx );

#endif
