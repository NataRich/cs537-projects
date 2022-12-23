#include "mapreduce.h"

typedef struct __mr_metadata
{
    int num_reducers;
    Mapper map;
    Reducer reduce;
    Partitioner partition;
} Meta; // Metadata

typedef struct __mr_cache
{
    LinkedList** partitions;
    size_t num_partitions;
} Cache; // Intermediate store

static Meta meta;
static Cache cache;

static void MR_MetaInit(Mapper map, int num_reducers, Reducer reduce,
                        Partitioner partition)
{
    meta.map = map;
    meta.num_reducers = num_reducers;
    meta.reduce = reduce;
    meta.partition = partition;
}

static void MR_CacheInit(size_t num_partitions)
{
    cache.num_partitions = num_partitions;
    cache.partitions = (LinkedList**) malloc(num_partitions * sizeof(LinkedList*));
    if (cache.partitions == NULL)
    {
        printf("ERROR: malloc failed (mapreduce.cacheinit.1).\n");
        exit(1);
    }

    for (size_t i = 0; i < num_partitions; i++)
        cache.partitions[i] = LinkedListInit();
}

static void MR_CacheFree()
{
    for (size_t i = 0; i < cache.num_partitions; i++)
        LinkedListFree(cache.partitions[i]);
    free(cache.partitions);
}

static void* MR_Map(void* arg)
{
    if (arg == NULL) printf("arg NULL\n");
    char* filename = (char*) arg;
    (*meta.map)(filename);
    return NULL;
}

static char* MR_GetNext(char* key, int partition_number)
{
    Node* next = LinkedListPeek(cache.partitions[partition_number]);
    if (next != NULL && strcmp(next->key, key) == 0)
    {
        LinkedListNext(cache.partitions[partition_number]);
        return next->key;
    }
    return NULL;
}

static void* MR_Reduce(void* arg)
{
    int partition_number= *(int*) arg;
    LinkedList* list = cache.partitions[partition_number];
    LinkedListSort(list);
    Node* next = LinkedListPeek(list);
    while (next != NULL)
    {
        (*meta.reduce)(next->key, MR_GetNext, partition_number);
        next = LinkedListPeek(list);
    }
    return NULL;
}

void MR_Emit(char *key, char *value)
{
    unsigned long partition_number = (*meta.partition)(key, meta.num_reducers);
    size_t val_size = sizeof(char) * (strlen(value) + 1);
    LinkedListAdd(cache.partitions[partition_number], key, value, val_size);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

void MR_Run(int argc, char *argv[], Mapper map, int num_mappers,
            Reducer reduce, int num_reducers, Partitioner partition)
{
    // Initialize
    MR_MetaInit(map, num_reducers, reduce, partition);
    MR_CacheInit(num_reducers);
    int num_files = argc - 1;

    // Map - stores all the kvs in the cache
    pthread_t m_threads[num_mappers];
    for (int i = 0; i < num_mappers; i++)
        if (i < num_files)
            pthread_create(&m_threads[i], NULL, MR_Map, argv[i + 1]);
    for (int i = 0; i < num_mappers; i++)
        if (i < num_files)
            pthread_join(m_threads[i], NULL);

    // Reduce - counts all the kvs in the cache
    pthread_t r_threads[num_reducers];
    int numbers[num_reducers];
    for (int i = 0; i < num_reducers; i++)
    {
        numbers[i] = i;
        pthread_create(&r_threads[i], NULL, MR_Reduce, &numbers[i]);
    }
    for (int i = 0; i < num_reducers; i++)
        pthread_join(r_threads[i], NULL);

    // Clear unused heap
    MR_CacheFree();
}
