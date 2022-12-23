#include "linkedlist.h"

ll_t *ll_init()
{
    ll_t *list = malloc(sizeof(ll_t));
    if (list == NULL)
    {
        printf("ERROR: ll_init.malloc failed\n");
        exit(1);
    }

    list->head = NULL;
    return list;
}

void ll_push(ll_t *list, f_jpg *jpg)
{
    node_t *new_head = malloc(sizeof(node_t));
    if (new_head == NULL)
    {
        printf("ERROR: ll_push.malloc failed\n");
        exit(1);
    }

    new_head->jpg = jpg;
    new_head->next = list->head;
    list->head = new_head;
}

f_jpg *ll_pop(ll_t *list)
{
    if (list == NULL)
    {
        printf("ERROR: invalid arguments (ll_pop)\n");
        exit(1);
    }

    if (list->head == NULL)
        return NULL;

    node_t *head = list->head;
    list->head = head->next;
    head->next = NULL;
    f_jpg *jpg = head->jpg;
    free(head);
    return jpg;
}

f_jpg *ll_get(ll_t *list, unsigned int inum)
{
    if (list == NULL || inum == 0)
    {
        printf("ERROR: invalid arguments (ll_get)\n");
        exit(1);
    }

    node_t *curr = list->head;
    while (curr != NULL)
    {
        if (inum == curr->jpg->inum)
            return curr->jpg;
        curr = curr->next;
    }
    return NULL;
}

void ll_free(ll_t *list)
{
    if (list == NULL) return;

    node_t *curr = list->head;
    while (curr != NULL)
    {
        jpg_free(curr->jpg);
        node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);
}

void ll_print(ll_t *list)
{
    if (list == NULL)
    {
        printf("NULL LIST\n");
        return;
    }

    node_t *curr = list->head;
    unsigned int counter = 0;
    while (curr != NULL)
    {
        printf("node %d:\n", counter++);
        jpg_print(curr->jpg);
        printf("====\n");
        curr = curr->next;
    }
}
