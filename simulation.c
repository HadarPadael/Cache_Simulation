#include "simulation.h"

// this method will run the simulation that the user will see and interact with.
void run() {
    int n;
    // the users first input is the size of the main memory for current simulation
    printf("Size of data: ");
    scanf("%d", &n);
    //allocate memory for main memory (mem)
    uchar *mem = malloc(n);
    //get some data for main memory from the user
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);
    //get the cache parameters s(number of set index bits), t(number of tag bits), b(number of block offset bits),
    // E(number of lines per set)
    int s, t, b, E;
    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);
    cache_t cache = initialize_cache(s, t, b, E);

    //the cache simulaiton starts. while the input is non-negative the input is read from memory to cache
    //otherwise, the simulation stops and the cache is printed to console.
    // in this simulation we assume that all input is valid, as per instructions.
    while (1) {
        scanf("%d", &n);
        if (n < 0)
            break;
        read_byte(cache, mem, n);
    }

    puts("");
    print_cache(cache);

    free(mem);
}