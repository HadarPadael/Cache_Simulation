#include "../headers/cache_funcs.h"
#include "../headers/cache_struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// this method writes to cache memory only.
// it should work as learnt in class:
// 1. calculate B (size of block array) out of b
// 2. check if off address is divident by B:
// if it is -> iterate from 0 to B-1 and in each slot fill the corresponding address starting from off to off + B - 1.
// if not -> change the starting address to be the first before off that is divident by B and iterate similarly to if case.
void write_to_cashe(cache_t cache, uchar *start, long int off, long int Y, long int X, int line) {
    // printf("writing...");
    int B = cache.b == 0 ? 1 : 1 << cache.b;
    long int address = off % B == 0 ? off : off - (off % B);
    for (int i = 0; i < B; ++i) {
        address = address + i;
        cache.cache[Y][line].block[i] = start[address];
    }
    cache.cache[Y][line].tag = X;
    cache.cache[Y][line].valid = 1;
}

// this method allocates variables from memory in case of cache miss.
// it maintains LFU and write allocate as clarified in the QA forum.
// it should work as follows:
// 1. divide off to XYZ
// 2. locate the Yth set:
// if (set is full) -> replace the LFU line with new variable and update its value to newC,
// in case all lines have the same frequency then the first line is replaced.
// else -> put the new variable in first available line and update its value to newC
void allocate_byte(cache_t cache, uchar *start, long int off, uchar newC) {
    // divide off to XYZ
    long int X, Y, Z;
    X = off >> (cache.b + cache.s);
    Y = (off >> cache.b) & ((1 << cache.s) - 1);
    Z = off & ((1 << cache.b) - 1);
    for (int i = 0; i < cache.E; ++i) {
        // printf("allocating..");
        if (cache.cache[Y][i].valid == 0) {
            // printf("found an empty line!");
            write_to_cashe(cache, start, off, Y, X, i);
            ++cache.cache[Y][i].frequency;
            cache.cache[Y][i].block[Z] = newC;
            return;
        }
    }
    // printf("nothings empty:(");
    uchar frequency = cache.cache[Y][0].frequency;
    int line = 0;
    for (int i = 1; i < cache.E; ++i) {
        // printf("looking for LFU...");
        if (cache.cache[Y][i].frequency < frequency) {
            // printf("found LFU!");
            frequency = cache.cache[Y][i].frequency;
            line = i;
        }
    }
    cache.cache[Y][line].frequency = 1;
    write_to_cashe(cache, start, off, Y, X, line);
}

// this method will create something like this in memory:
// cache.cache ---> [ set 0 ] -> [ cache line 0 ] -> { valid, frequency, tag, block{B} }
//                 |           -> [ cache line 1 ] -> { valid, frequency, tag, block{B} }
//                 |            .....
//                 |           -> [ cache line E-1 ] -> { valid, frequency, tag, block{B} }
//                 |
//                 -> [ set 1 ] -> [ cache line 0 ] -> { valid, frequency, tag, block{B} }
//                 |            -> [ cache line 1 ] -> { valid, frequency, tag, block{B} }
//                 |             .....
//                 |            -> [ cache line E-1 ] -> { valid, frequency, tag, block{B} }
//                 |
//                 ......
//                 -> [ set S-1 ] -> [ cache line 0 ] -> { valid, frequency, tag, block{B} }
//                 |              -> [ cache line 1 ] -> { valid, frequency, tag, block{B} }
//                 |               .....
//                 |              -> [ cache line E-1 ] -> { valid, frequency, tag, block{B} }

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    // initialize the cache structure
    cache_t cache = {s, t, b, E, NULL};
    // calculate S and B
    int S = s == 0 ? 1 : 1 << s;
    int B = b == 0 ? 1 : 1 << b;
    // allocate contiguous memory for S pointers, set initial values to zero
    cache.cache = calloc(S, sizeof(cache_line_t *));
    // initialize each set
    for (int i = 0; i < S; ++i) {
        cache.cache[i] = calloc(sizeof(cache_line_t), cache.E);
        for (int j = 0; j < cache.E; ++j) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            cache.cache[i][j].block = malloc(B);
        }
    }
    return cache;
}

// this method gets the cache we initialized in the previous function, the starting position of our simulated memory,
// and the location in memory which we want to read from. It should work as we learned in class:
// 1. divide off to XYZ
// 2. search for Z in the Yth set of cache:
// if (Z is found) -> return its value as uchar,
// else -> allocate start[off] from memory to cashe in set Y and return its value as uchar.

uchar read_byte(cache_t cache, uchar *start, long int off) {

    // divide off to XYZ
    long int X, Y, Z;
    X = off >> (cache.b + cache.s);
    Y = (off >> cache.b) & ((1 << cache.s) - 1);
    Z = off & ((1 << cache.b) - 1);
    // search for Z in the Yth set of cache
    for (int i = 0; i < cache.E; ++i) {
        // printf("searching....");
        if (cache.cache[Y][i].tag == X && cache.cache[Y][i].valid == 1) {
            // printf("found it!");
            ++cache.cache[Y][i].frequency;
            return cache.cache[Y][i].block[Z];
        }
    }
    // printf("going to get it");
    allocate_byte(cache, start, off, start[off]);
    return start[off];
}

// this method writes a new value to a variable of the program. As clarified in the QA forum,
// it sould maintain write through + write allocate methods.
// it should work as follows:
// 1. divide off to XYZ
// 2. search for Z in the Yth set of cache:
// if (Z is found) -> rewrite its value in cashe as well as in memory.
// else -> allocate start[off] from memory to cashe and rewrite its value there (only).
void write_byte(cache_t cache, uchar *start, long int off, uchar newC) {
    // divide off to XYZ
    long int X, Y, Z;
    X = off >> (cache.b + cache.s);
    Y = (off >> cache.b) & ((1 << cache.s) - 1);
    Z = off & ((1 << cache.b) - 1);
    // search for Z in the Yth set of cache
    for (int i = 0; i < cache.E; ++i) {
        if (cache.cache[Y][i].tag == X && cache.cache[Y][i].valid == 1) {
            ++cache.cache[Y][i].frequency;
            cache.cache[Y][i].block[Z] = newC;
            start[off] = newC;
            break;
        }
        else {
            allocate_byte(cache, start, off, newC);
        }
    }
}

void print_cache(cache_t cache) {
    int S = 1 << cache.s;
    int B = 1 << cache.b;

    for (int i = 0; i < S; i++) {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++) {
            printf("%1d %d 0x%0*lx ", cache.cache[i][j].valid,
                   cache.cache[i][j].frequency, cache.t, cache.cache[i][j].tag);
            for (int k = 0; k < B; k++) {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}