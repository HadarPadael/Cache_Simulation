#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

typedef unsigned char uchar;

typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar *block;
} cache_line_t;

typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t **cache;
} cache_t;

#endif