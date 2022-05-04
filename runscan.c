#include <stdio.h>
#include "ext2_fs.h"
#include "read_ext2.h"
#include "linkedlist.h"
#include "reader.h"

// Returns 1 if it's a jpg, otherwise 0.
int is_file_jpg(char buffer[1024]) {
  if (buffer[0] == (char)0xff && buffer[1] == (char)0xd8 && 
      buffer[2] == (char)0xff && (buffer[3] == (char)0xe0 ||
      buffer[3] == (char)0xe1 || buffer[3] == (char)0xe8)) {
    return 1;
  }
  return 0;
}

int test_data_block(int fd, off_t offset) {
  char buffer[1024];
  seek_and_read_block(fd, offset, (void *)buffer);
  return is_file_jpg(buffer);
}

// called after ext2_read_init()
void scan_inode_table_multi(int fd, ll_t *out_list) {
  unsigned int inum = 1;

  for (unsigned int ngroup = 0; ngroup < num_groups; ngroup++) {
    struct ext2_super_block super;
    struct ext2_group_desc group;
    read_super_block(fd, ngroup, &super);
    read_group_desc(fd, ngroup, &group);

    off_t start_inode_table = locate_inode_table(ngroup, &group);
    for (unsigned int i = 1; i < inodes_per_group; ++i, ++inum) {
      struct ext2_inode *inode = malloc(sizeof(struct ext2_inode));
      read_inode(fd, ngroup, start_inode_table, i, inode);

      if (S_ISREG(inode->i_mode) && inode->i_size > 0)  // a file with content
        if (test_data_block(fd, BLOCK_OFFSET(inode->i_block[0])))  // a jpg file
          ll_push(out_list, read_jpg_inode(fd, inode, &super, inum));

      free(inode);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("expected usage: ./runscan inputfile outputfile\n");
    exit(0);
  }

  int fd = open(argv[1], O_RDONLY);
  ext2_read_init(fd);

  ll_t * list = ll_init();
  scan_inode_table_multi(fd, list);
  ll_print(list);
  ll_free(list);

  return 0;
}
