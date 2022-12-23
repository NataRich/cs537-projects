#include "jpg.h"

f_jpg *jpg_create(unsigned int inum)
{
    f_jpg *jpg = malloc(sizeof(f_jpg));
    if (jpg == NULL)
    {
        printf("ERROR: jpg_create.malloc failed\n");
        exit(1);
    }

    jpg->inum = inum;
    jpg->filename = NULL;
    jpg->data_blocks = NULL;
    jpg->block_size = 0;
    return jpg;
}

void jpg_add_filename(f_jpg *jpg, const char *name, unsigned char len)
{
    if (jpg == NULL || name == NULL || len == 0)
    {
        printf("ERROR: invalid arguments (jpg_add_filename)\n");
        exit(1);
    }

    if (jpg->filename == NULL)
    {
        jpg->filename = malloc(255 * sizeof(char));
        strncpy(jpg->filename, name, len);
        jpg->filename[len] = '\0';
    }
    else
    {
        printf("DEBUG: filename already existed\n");
    }
}

void jpg_add_data_block(f_jpg *jpg, __u32 bnum)
{
    if (jpg == NULL)
    {
        printf("ERROR: invalid arguments (jpg_add_data_block)\n");
        exit(1);
    }

    size_t size = ++jpg->block_size * sizeof(__u32);
    jpg->data_blocks = realloc(jpg->data_blocks, size);
    if (jpg->data_blocks == NULL)
    {
        printf("ERROR: jpg_add_data_block.realloc failed\n");
        exit(1);
    }
    jpg->data_blocks[jpg->block_size - 1] = bnum;
}

void jpg_free(f_jpg *jpg)
{
    if (jpg != NULL)
    {
        if (jpg->filename != NULL)
            free(jpg->filename);
        if (jpg->block_size != 0)
            free(jpg->data_blocks);
        jpg->inum = 0;
        jpg->block_size = 0;
        free(jpg);
    }
}

void jpg_print(f_jpg *jpg)
{
    if (jpg != NULL)
    {
        printf(" inum: %d\n", jpg->inum);
        printf(" filename: %s\n", jpg->filename);
        printf(" block size: %ld\n", jpg->block_size);
        for (size_t i = 0; i < jpg->block_size; ++i)
            printf("  block #%ld: %d\n", i, jpg->data_blocks[i]);
    }
    else
    {
        printf("DEBUG: jpg is NULL\n");
    }
}
