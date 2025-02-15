#ifndef CACHE_FUNCS_H
#define CACHE_FUNCS_H

#include "cache_struct.h"

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);

uchar read_byte(cache_t cache, uchar *start, long int off);

void write_byte(cache_t cache, uchar *start, long int off, uchar newC);

void print_cache(cache_t cache);

#endif