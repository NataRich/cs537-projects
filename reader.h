#ifndef READER_H
#define READER_H

#include "ext2_fs.h"
#include "read_ext2.h"
#include "linkedlist.h"

#define MAX_EXT2_IND_BLOCKS 256
#define MAX_EXT2_DIND_BLOCKS 65536

void seek_and_read_block(int fd, off_t offset, void *buffer);

f_jpg *read_jpg_inode(int fd,
                      struct ext2_inode *inode,
                      struct ext2_super_block *super,
                      unsigned int inum);

#endif  // READER_H
