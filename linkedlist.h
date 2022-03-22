#ifndef __linkedlist_h_
#define __linkedlist_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct __kv_pair
{
    char* key;
    void* val;
    size_t val_size;
    struct __kv_pair* next;
} Node;

typedef struct __cc_linkedlist
{
    Node* head;
    Node** array;
    size_t size;
    int sorted;
    int next; // index
    pthread_rwlock_t lock;
} LinkedList;

LinkedList* LinkedListInit();
void* LinkedListFind(LinkedList* list, const char* key);
void LinkedListSet(LinkedList* list, const char* key, const void* val,
                   size_t val_size);
void LinkedListAdd(LinkedList* list, const char* key, const void* val,
                   size_t val_size);
void LinkedListSort(LinkedList* list);
void LinkedListNext(LinkedList* list);
Node* LinkedListPeek(LinkedList* list);
void LinkedListFree(LinkedList* list);


#endif
