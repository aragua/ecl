#include "internals.h"

#include "setip.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
 
int ecl_set_ip(char *iface_name, char *ip_addr)
{
	if(!iface_name)
		return -1;	
 
	int sockfd;
	struct ifreq ifr;
	struct sockaddr_in sin;
	
	LOG( LOG_INFO, "Ca marche\n");
 
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1)
	{
		LOG( LOG_ERR, "Could not get socket.\n");
		return -1;
	}
 
	/* get interface name */
	strncpy(ifr.ifr_name, iface_name, IFNAMSIZ);
 
	/* Read interface flags */
	if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0)
	{
		LOG( LOG_ERR, "ifdown: shutdown : %s \n", strerror(errno));
		return -1;
	}

	// If interface is down, bring it up
	if (!(ifr.ifr_flags & IFF_UP)) 
	{
		LOG( LOG_ERR, "Device is currently down..setting up.-- %u\n",ifr.ifr_flags);
		ifr.ifr_flags |= IFF_UP;
		if (ioctl(sockfd, SIOCSIFFLAGS, &ifr) < 0) 
		{
			LOG( LOG_ERR, "ifup: failed : %s \n", strerror(errno));
			return -1;
		}
	}
 
	sin.sin_family = AF_INET;
 
	// Convert IP from numbers and dots to binary notation
	inet_aton(ip_addr,&sin.sin_addr);	
	memcpy(&ifr.ifr_addr, &sin, sizeof(struct sockaddr));	
 
	// Set interface address
	if (ioctl(sockfd, SIOCSIFADDR, &ifr) < 0)
	{
		LOG( LOG_ERR, "Cannot set IP address : %s \n", strerror(errno));
		return -1;
	}
	
	close(sockfd);
	
	return 0;
}

