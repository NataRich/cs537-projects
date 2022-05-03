#ifndef _JPG_H_
#define _JPG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _jpg_data_ {
    unsigned int inum;
    char *filename;
    unsigned int *data_blocks;
    size_t block_size;
} f_jpg;

f_jpg *jpg_create(unsigned int inum);

void jpg_add_filename(f_jpg *jpg, const char *name, unsigned char len);

void jpg_add_data_block(f_jpg *jpg, unsigned int bnum);

void jpg_free(f_jpg *jpg);

// DEBUG
void jpg_print(f_jpg *jpg);

#endif // _JPG_H_
