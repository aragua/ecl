#ifndef YOCTOWEB_PARAM_H
#define YOCTOWEB_PARAM_H

#include "Array.h"

typedef struct
{
  char * key;
  char * value;
} Pair;

static inline Pair * Pair_new( const char * key, const char * value )
{
  Pair * p = malloc(sizeof(Pair));
  memset(p,0,sizeof(Pair));
  if ( p )
    {
      if ( key )
	p->key = strdup(key);
      if ( value )
	p->value = strdup(value);
    }
  return p;
}

static inline void Pair_free( Pair * p)
{
  if(p)
    {
      if (p->key)
	free(p->key);
      if (p->value)
	free(p->value);
      free(p);
    }
}

typedef Array Dictionnary;

static inline Dictionnary * Dictionnary_new(void)
{
  return (Dictionnary *) array_new();
}

static inline int Dictionnary_add(Dictionnary*d, Pair *p)
{
  return array_add((Array*)d,(void*)p);
}

static inline void Dictionnary_free(Dictionnary*d)
{
  array_free((Array*)d);
}

static inline char * Dictionnary_value_for_key( Dictionnary * d, const char * key)
{
  int idx;
  
  if ( !key )
    return NULL;

  for ( idx = 0 ; idx < d->count ; idx++ )
    {
      if ( ((Pair*)(d->table[idx]))->key && strcmp(key, ((Pair*)(d->table[idx]))->key) == 0 )
	return ((Pair*)(d->table[idx]))->value;
    }

  return NULL;
}

static inline void Dictionnary_show( Dictionnary * d)
{
  int idx;
  for ( idx = 0 ; idx < d->count ; idx++ )
    {
      if ( ((Pair*)(d->table[idx]))->key && ((Pair*)(d->table[idx]))->value )
	printf("%s: %s\n", ((Pair*)(d->table[idx]))->key, ((Pair*)(d->table[idx]))->value );
    }
}

/* output scheme: <d1>key<d2>value<d3> */
static inline char * Dictionnary_to_string(Dictionnary * d,char d1,char d2,char d3)
{
  char * ret = NULL,*cursor;
  int idx, size=1;

  for ( idx = 0 ; idx < d->count ; idx++ )
    {
      if ( ((Pair*)(d->table[idx]))->key && ((Pair*)(d->table[idx]))->value )
	{
	  size += 1+strlen(((Pair*)(d->table[idx]))->key)+1+strlen(((Pair*)(d->table[idx]))->value)+1;
	}
    }
  
  ret = malloc(size);
  if ( ret )
    {
      cursor=ret;
      for ( idx = 0 ; idx < d->count ; idx++ )
	{
	  if ( ((Pair*)(d->table[idx]))->key && ((Pair*)(d->table[idx]))->value )
	    {
	      *cursor=d1;
	      cursor++;
	      memcpy(cursor,((Pair*)(d->table[idx]))->key,strlen(((Pair*)(d->table[idx]))->key));
	      cursor+=strlen(((Pair*)(d->table[idx]))->key);
	      *cursor=d2;
	      cursor++;
	      memcpy(cursor,((Pair*)(d->table[idx]))->value,strlen(((Pair*)(d->table[idx]))->value));
	      cursor+=strlen(((Pair*)(d->table[idx]))->value);
	      *cursor=d3;
	      cursor++;
	    }
	}
      *cursor=0;
    }

  return ret;
}


#endif
