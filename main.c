#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jpg.h"
#include "linkedlist.h"

int main(int argc, char *argv[])
{
    ll_t *list = ll_init();
    f_jpg *jpg = jpg_create(3);
    jpg_add_filename(jpg, "hello world", 11);
    jpg_add_data_block(jpg, 5);
    ll_push(list, jpg);
    ll_print(list);
    f_jpg *head = ll_pop(list);
    jpg_free(head);
    ll_free(list);

    return 0;
}
