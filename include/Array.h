#ifndef YOCTOWEB_ARRAY_H
#define YOCTOWEB_ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ARRAY_DEFAULT_SIZE 128

typedef struct
{
  void ** table;
  int count;
  int table_sz;
} Array;

static inline Array * array_new(void)
{
  Array * a = NULL;

  a = malloc(sizeof(Array));
  if ( a )
    {
      a->table_sz = ARRAY_DEFAULT_SIZE;
      a->count = 0;
      a->table = malloc(sizeof(void*)*ARRAY_DEFAULT_SIZE);
      if (!a->table)
	{
	  free(a);
	  a = NULL;
	}
    }
    return a;
}

static inline int array_add(Array*a, void*p)
{
  if (!a||!p)
    return -1;
  
  if ( a->count >= a->table_sz )
    {
      a->table = realloc(a->table,a->table_sz+ARRAY_DEFAULT_SIZE);
      if ( !a->table )
	{
	  perror("realloc");
	  exit(ENOMEM);
	}
      a->table_sz += ARRAY_DEFAULT_SIZE;
    }

  a->table[a->count] = p;
  a->count++;

  return 0;
}

static inline void * array_at(Array *a, int idx)
{
  if ( !a || idx < 0 || idx > a->count )
    return NULL;
  return a->table[idx];
}

static inline void array_free(Array *a)
{
  if ( a )
    {
      if ( a->table )
	{
	  int idx;
	  for ( idx = 0; idx < a->count; idx++ )
	    {
	      if (a->table[idx])
		free(a->table[idx]);
	    }
	  free(a->table);
	}
      free(a);
    }
}

#endif
