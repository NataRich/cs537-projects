#include "writer.h"

int dir_exists(char *path) {
  struct stat st = {0};
  return stat(path, &st) != -1;
}

void write_jpg(int fd, char *dir_path, f_jpg *jpg) {
  if (!dir_exists(dir_path))
    mkdir(dir_path, PERM);

  // write inum name
  char path1[1024], path2[1024];
  snprintf(path1, 1024, "%s/file-%d.jpg", dir_path, jpg->inum);
  snprintf(path2, 1024, "%s/%s", dir_path, jpg->filename);
  int out_fd1 = open(path1, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  int out_fd2 = open(path2, O_WRONLY | O_TRUNC | O_CREAT, 0666);
  for (size_t i = 0; i < jpg->block_size; i++) {
    size_t write_size = block_size;
    if (i + 1 == jpg->block_size)
      write_size = jpg->file_size % block_size;

    char buffer[1024];
    seek_and_read_block(fd, BLOCK_OFFSET(jpg->data_blocks[i]), buffer);
    write(out_fd1, buffer, write_size);
    write(out_fd2, buffer, write_size);
  }

  close(out_fd1);
  close(out_fd2);
}
