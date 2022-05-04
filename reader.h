#ifndef READER_H
#define READER_H

#include "ext2_fs.h"
#include "read_ext2.h"
#include "linkedlist.h"

#define MAX_EXT2_IND_BLOCKS 256
#define MAX_EXT2_DIND_BLOCKS 65536

#define ALIGN(n) (n%4 ? (n/4+1)*4 : n)

void seek_and_read_block(int fd, off_t offset, void *buffer);

f_jpg *read_jpg_inode(int fd, struct ext2_inode *inode, unsigned int inum);

void read_dir_entry(int fd, struct ext2_inode *inode, ll_t *list);

#endif  // READER_H
