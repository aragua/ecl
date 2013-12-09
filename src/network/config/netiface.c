#include "netiface.h"

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>

int ecl_get_iface( iface_list_t * list )
{
	char          buf[1024];
	struct ifconf ifc;
	struct ifreq *ifr;
	int           sock;
	int           idx;

	if ( !list )
		return -1;

	/* Get a socket handle. */
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0)
	{
		perror("socket");
		return 1;
	}

	/* Query available interfaces. */
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if(ioctl(sock, SIOCGIFCONF, &ifc) < 0)
	{
		perror("ioctl(SIOCGIFCONF)");
		return 1;
	}

	/* Iterate through the list of interfaces. */
	ifr         = ifc.ifc_req;
	list->count = ifc.ifc_len / sizeof(struct ifreq);
	list->name = malloc( list->count * sizeof(char *) );
	for(idx = 0; idx < list->count; idx++)
	{
		/* Get iface name */
		list->name[idx] = strdup( ((struct ifreq *)&ifr[idx])->ifr_name);
		/* Get IP address */
		// inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr));
		/* Get MAC address */
		// ioctl(sock, SIOCGIFHWADDR, item);
		/* Get broadcast address */
		// ioctl(sock, SIOCGIFBRDADDR, item);
		// inet_ntoa(((struct sockaddr_in *)&item->ifr_broadaddr)->sin_addr));
	}

        return 0;
}

void ecl_clean_iface( iface_list_t * list )
{
	int idx;
	if ( list )
		for(idx = 0; idx < list->count; idx++)
			if ( list->name[idx] )
				free(list->name[idx]);
}
