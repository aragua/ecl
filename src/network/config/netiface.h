#ifndef SRC_NETWORK_CONFIG_LISTIF_H
#define SRC_NETWORK_CONFIG_LISTIF_H

typedef struct
{
	int count;
	char ** name;
} iface_list_t;

int ecl_get_iface( iface_list_t * list );
void ecl_clean_iface( iface_list_t * list );

#endif
