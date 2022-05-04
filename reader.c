#include "reader.h"

void seek_and_read_block(int fd, off_t offset, void *buffer) {
  lseek(fd, offset, SEEK_SET);
  read(fd, buffer, block_size);
}

void read_direct(struct ext2_inode *inode, unsigned int *n_blocks, f_jpg *jpg) {
  if (*n_blocks <= 0)
    return;

  unsigned int boundary = *n_blocks;
  if (boundary > EXT2_NDIR_BLOCKS)
    boundary = EXT2_NDIR_BLOCKS;
  *n_blocks -= boundary;

  for (unsigned int i = 0; i < boundary; ++i)
    jpg_add_data_block(jpg, inode->i_block[i]);
}

void read_indirect(int fd, __u32 block_num, unsigned int *n_blocks, f_jpg *jpg) {
  if (*n_blocks <= 0)
    return;

  unsigned int boundary = *n_blocks;
  if (boundary > MAX_EXT2_IND_BLOCKS)
    boundary = MAX_EXT2_IND_BLOCKS;
  *n_blocks -= boundary;

  // read the indirect block and get the block numbers
  __u32 block_nums[MAX_EXT2_IND_BLOCKS];
  off_t offset = BLOCK_OFFSET(block_num);
  seek_and_read_block(fd, offset, (void *)block_nums);

  // store block numbers
  for (unsigned int i = 0; i < boundary; ++i)
    jpg_add_data_block(jpg, block_nums[i]);
}

void read_double_indirect(int fd, __u32 block_num, unsigned int *n_blocks, f_jpg *jpg) {
  if (*n_blocks <= 0)
    return;

  if (*n_blocks > MAX_EXT2_DIND_BLOCKS) {
    // remainig data blocks can only be found from the triple indirect block, but for
    // this project, this should not happen
    printf("ERROR: shouldn't need triple indirect\n");
    exit(1);
  }

  unsigned int remaining = *n_blocks % MAX_EXT2_IND_BLOCKS;
  unsigned int block_boundary = *n_blocks / MAX_EXT2_IND_BLOCKS;
  if (remaining)
    block_boundary += 1;
  *n_blocks = 0;

  // get destination block numbers from intermediate block
  __u32 dest_block_nums[MAX_EXT2_IND_BLOCKS];
  // get offset of the intermediate block
  off_t intermediate_offset = BLOCK_OFFSET(block_num);
  seek_and_read_block(fd, intermediate_offset, (void *)dest_block_nums);

  __u32 block_nums[MAX_EXT2_IND_BLOCKS];
  for (unsigned int i = 0; i < block_boundary; ++i) {
    // get offset of each destination block
    off_t offset = BLOCK_OFFSET(dest_block_nums[i]);
    seek_and_read_block(fd, offset, (void *)block_nums);

    unsigned int boundary = MAX_EXT2_IND_BLOCKS;
    if (i + 1 == block_boundary && remaining)
      boundary = remaining;

    // store block numbers
    for (unsigned int j = 0; j < boundary; ++j)
      jpg_add_data_block(jpg, block_nums[j]);
  }
}

f_jpg *read_jpg_inode(int fd,
                      struct ext2_inode *inode,
                      struct ext2_super_block *super,
                      unsigned int inum) {
  f_jpg *jpg = jpg_create(inum);
  jpg->inum = inum;

  // read data block numbers
  unsigned int n_blocks = inode->i_blocks / (2 << super->s_log_block_size);
  // 1) read direct
  read_direct(inode, &n_blocks, jpg);
  // 2) read indirect
  read_indirect(fd, inode->i_block[12], &n_blocks, jpg);
  // 3) read double indirect
  read_double_indirect(fd, inode->i_block[13], &n_blocks, jpg);

  return jpg;
}
