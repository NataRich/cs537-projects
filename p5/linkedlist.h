#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpg.h"

typedef struct _node_t {
    f_jpg *jpg;
    struct _node_t *next;
} node_t;

typedef struct _list_t {
    node_t *head;
} ll_t;

ll_t *ll_init();
void ll_push(ll_t *list, f_jpg *jpg);
f_jpg *ll_pop(ll_t *list);
f_jpg *ll_get(ll_t *list, unsigned int inum);
void ll_free(ll_t *list);

// DEBUG
void ll_print(ll_t *list);

#endif // _LINKEDLIST_H_
