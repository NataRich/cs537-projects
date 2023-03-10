#include "reader.h"

void seek_and_read_block(int fd, off_t offset, void *buffer) {
  lseek(fd, offset, SEEK_SET);
  read(fd, buffer, block_size);
}

void read_direct(struct ext2_inode *inode, unsigned int n_blocks, f_jpg *jpg) {
  if (n_blocks <= 0)
    return;

  unsigned int boundary;
  if (n_blocks >= EXT2_NDIR_BLOCKS)
    boundary = EXT2_NDIR_BLOCKS;
  else
    boundary = n_blocks;

  for (unsigned int i = 0; i < boundary; ++i)
    jpg_add_data_block(jpg, inode->i_block[i]);
}

void read_indirect(int fd, __u32 block_num, unsigned int n_blocks, f_jpg *jpg) {
  if (n_blocks <= EXT2_NDIR_BLOCKS)
    return;

  unsigned int boundary;
  if (n_blocks >= MAX_EXT2_IND_BLOCKS + EXT2_NDIR_BLOCKS)
    boundary = MAX_EXT2_IND_BLOCKS;
  else
    boundary = n_blocks - EXT2_NDIR_BLOCKS;

  // read the indirect block and get the block numbers
  __u32 block_nums[MAX_EXT2_IND_BLOCKS];
  off_t offset = BLOCK_OFFSET(block_num);
  seek_and_read_block(fd, offset, (void *)block_nums);

  // store block numbers
  for (unsigned int i = 0; i < boundary; ++i)
    jpg_add_data_block(jpg, block_nums[i]);
}

void read_double_indirect(int fd, __u32 block_num, unsigned int n_blocks, f_jpg *jpg) {
  if (n_blocks <= MAX_EXT2_IND_BLOCKS + EXT2_NDIR_BLOCKS)
    return;

  if (n_blocks > MAX_EXT2_DIND_BLOCKS + MAX_EXT2_IND_BLOCKS + EXT2_NDIR_BLOCKS) {
    // remainig data blocks can only be found from the triple indirect block, but for
    // this project, this should not happen
    printf("ERROR: shouldn't need triple indirect\n");
    exit(1);
  }

  n_blocks = n_blocks - MAX_EXT2_IND_BLOCKS - EXT2_NDIR_BLOCKS;
  unsigned int remaining = n_blocks % MAX_EXT2_IND_BLOCKS;
  unsigned int block_boundary = n_blocks / MAX_EXT2_IND_BLOCKS;
  if (remaining)
    block_boundary += 1;

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

f_jpg *read_jpg_inode(int fd, struct ext2_inode *inode, unsigned int inum) {
  f_jpg *jpg = jpg_create(inum);
  jpg->inum = inum;
  jpg->file_size = inode->i_size;

  // read data block numbers
  unsigned int n_blocks = inode->i_size / block_size;
  if (inode->i_size % block_size)
    n_blocks += 1;

  // 1) read direct
  read_direct(inode, n_blocks, jpg);
  // 2) read indirect
  read_indirect(fd, inode->i_block[12], n_blocks, jpg);
  // 3) read double indirect
  read_double_indirect(fd, inode->i_block[13], n_blocks, jpg);

  return jpg;
}

void read_dir_entry(int fd, struct ext2_inode *inode, ll_t *list) {
  char buffer[1024];
  // get the offset of the first data block
  off_t offset = BLOCK_OFFSET(inode->i_block[0]);
  seek_and_read_block(fd, offset, buffer);

  unsigned int off = 24;
  while (off < block_size) {
    struct ext2_dir_entry_2 *entry = (struct ext2_dir_entry_2 *)(buffer + off);
    if (entry->inode != 0) {
      f_jpg *jpg = ll_get(list, entry->inode);
      if (jpg != NULL)
        jpg_add_filename(jpg, entry->name, entry->name_len & 0xFF);
    }
    off += sizeof(struct ext2_dir_entry_2) + ALIGN(entry->name_len);
  }
}
