#ifndef __hashmap_h__
#define __hashmap_h__

#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

#define N_BUCKETS 32

typedef struct __cc_hashmap
{
    LinkedList** buckets;
    size_t num_buckets;
    size_t size;
} HashMap;

// External Functions
HashMap* MapInit(void);
void MapPut(HashMap* map, char* key, void* value, int value_size);
char* MapGet(HashMap* map, char* key);
size_t MapSize(HashMap* map);

// Internal Functions
int resize_map(HashMap* map);
size_t Hash(char* key, size_t capacity);

#endif // __hashmap_h__
