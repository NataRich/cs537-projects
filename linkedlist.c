#include "linkedlist.h"

LinkedList* LinkedListInit()
{
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    if (list == NULL)
    {
        printf("ERROR: malloc failed (linkedlist.init.1).\n");
        exit(1);
    }
    list->head = NULL;
    list->array = NULL;
    list->size = 0;
    list->sorted = 0;
    list->next = 0;
    pthread_rwlock_init(&list->lock, NULL);
    return list;
}

void* LinkedListFind(LinkedList* list, const char* key)
{
    void* val = NULL;
    pthread_rwlock_rdlock(&list->lock);
    Node* curr = list->head;
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
            break;
        curr = curr->next;
    }

    // Key present
    if (curr != NULL)
    {
        val = malloc(curr->val_size);
        if (val == NULL)
        {
            printf("ERROR: malloc failed (linkedlist.find.1).\n");
            exit(1);
        }
        memcpy(val, curr->val, curr->val_size);
    }
    pthread_rwlock_unlock(&list->lock);
    return val;
}

static void LinkedListAddToHead(LinkedList* list, const char* key,
                                const void* val, size_t val_size)
{
    Node* new_head = (Node*) malloc(sizeof(Node));
    if (new_head == NULL)
    {
        printf("ERROR: malloc failed (linkedlist.addtohead.1).\n");
        exit(1);
    }
    new_head->key = strdup(key);
    new_head->val = malloc(val_size);
    if (new_head->val == NULL)
    {
        printf("ERROR: malloc failed (linkedlist.addtohead.2).\n");
        exit(1);
    }
    memcpy(new_head->val, val, val_size);
    new_head->val_size = val_size;
    new_head->next = list->head;
    list->head = new_head;
    list->size++;
}

void LinkedListSet(LinkedList* list, const char* key, const void* val, 
                   size_t val_size)
{
    pthread_rwlock_wrlock(&list->lock);
    Node* curr = list->head;
    while (curr != NULL)
    {
        if (strcmp(curr->key, key) == 0)
            break;
        curr = curr->next;
    }

    // If the key is absent in the list, then add a new node
    if (curr == NULL)
        LinkedListAddToHead(list, key, val, val_size);

    // If the key is present in the list, then set the new value
    if (curr != NULL)
    {
        curr->val = realloc(curr->val, val_size);
        if (curr->val == NULL)
        {
            printf("ERROR: realloc failed (linkedlist.set.1).\n");
            exit(1);
        }
        memcpy(curr->val, val, val_size);
        curr->val_size = val_size;
    }

    list->sorted = 0; // always unsorted after insertion
    pthread_rwlock_unlock(&list->lock);
}

void LinkedListAdd(LinkedList* list, const char* key, const void* val,
                            size_t val_size)
{
    pthread_rwlock_wrlock(&list->lock);
    LinkedListAddToHead(list, key, val, val_size);
    list->sorted = 0; // always unsorted after insertion
    pthread_rwlock_unlock(&list->lock);
}

static int compar(const void* node1, const void* node2)
{
    const char* k1 = (*(const Node**) node1)->key;
    const char* k2 = (*(const Node**) node2)->key;
    return strcmp(k1, k2);
}

void LinkedListSort(LinkedList* list)
{
    pthread_rwlock_wrlock(&list->lock);
    if (list->sorted == 0 && list->size > 0)
    {
        list->array = (Node**) realloc(list->array, list->size * sizeof(Node*));
        if (list->array == NULL)
        {
            printf("ERROR: realloc failed (linkedlist.sort.1).\n");
            exit(1);
        }

        int index = 0;
        Node* curr = list->head;
        while (curr != NULL)
        {
            list->array[index++] = curr;
            curr = curr->next;
        }

        qsort(list->array, list->size, sizeof(Node*), compar);
        list->sorted = 1;
        list->next = 0;
    }
    pthread_rwlock_unlock(&list->lock);
}

void LinkedListNext(LinkedList* list)
{
    pthread_rwlock_wrlock(&list->lock);
    list->next++;
    pthread_rwlock_unlock(&list->lock);
}

Node* LinkedListPeek(LinkedList* list)
{
    Node* node = NULL;
    pthread_rwlock_rdlock(&list->lock);
    if (list->array != NULL && list->sorted && list->next < list->size)
        node = list->array[list->next];
    pthread_rwlock_unlock(&list->lock);
    return node; 
}

void LinkedListFree(LinkedList* list)
{
    Node* curr = list->head;
    Node* next = NULL;
    while (curr != NULL)
    {
        next = curr->next;
        free(curr->key);
        free(curr->val);
        free(curr);
        curr = next;
    }
    free(list->array);
    pthread_rwlock_destroy(&list->lock);
    free(list);
}
