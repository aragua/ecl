#include "internals.h"

void usage( char * pname)
{
	LOG( LOG_ERR, "%s %s", pname, "[interface] [ip address]\n");
}
 
int main(int argc, char **argv)
{
	if(argc < 3)
	{
		usage(argv[0]);
		return -1;
	}
 
	ecl_set_ip(argv[1],argv[2]);
 
	return 0;
}
