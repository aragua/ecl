#ifndef SRC_NETWORK_TCP_CLIENT_H
#define SRC_NETWORK_TCP_CLIENT_H

typedef int (*tcp_client_callback)( int sock, void * params );

/**
  * This function create a client and call a function to use this socket.
  * then it closes the socket
  */
int ecl_start_tcp_client( char * ip, short port, tcp_client_callback callback, void * params );

#endif
