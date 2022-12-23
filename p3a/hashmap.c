#include "hashmap.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

size_t Hash(char* key, size_t num_buckets)
{
    size_t hash = FNV_OFFSET;
    for (const char *p = key; *p; p++) {
        hash ^= (size_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
        hash ^= (size_t)(*p);
    }
    return (hash % num_buckets);
}

HashMap* MapInit(void)
{
    HashMap* map = (HashMap*) malloc(sizeof(HashMap));
    if (map == NULL)
    {
        printf("ERROR: malloc failed (hashmap.init.1).\n");
        exit(1);
    }
    map->buckets = (LinkedList**) malloc(N_BUCKETS * sizeof(LinkedList*));
    if (map->buckets == NULL)
    {
        printf("ERROR: malloc failed (hashmap.init.2).\n");
        exit(1);
    }
    for (size_t i = 0; i < N_BUCKETS; i++)
        map->buckets[i] = LinkedListInit();
    map->num_buckets = N_BUCKETS;
    return map;
}

void MapPut(HashMap* map, char* key, void* val, int val_size)
{
    LinkedListSet(map->buckets[Hash(key, map->num_buckets)], key, val, val_size);
}

char* MapGet(HashMap* map, char* key)
{
    return LinkedListFind(map->buckets[Hash(key, map->num_buckets)], key);
}

size_t MapSize(HashMap* map)
{
    size_t size = 0;
    for (size_t i = 0; i < map->num_buckets; i++)
        size += LinkedListGetSize(map->buckets[i]);
    return size;
}

int resize_map(HashMap* map)
{
    return 0;
}
