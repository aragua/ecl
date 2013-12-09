#include "internals.h"

int main(void)
{
	int idx;
	iface_list_t list;
	ecl_get_iface( &list );
	printf("Interfaces:\n");
	for( idx = 0; idx < list.count ; idx++ )
		printf("\t%s\n", list.name[idx] );
	ecl_clean_iface( &list );

        return 0;
}
