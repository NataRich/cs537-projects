#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "jpg.h"
#include "ext2_fs.h"
#include "read_ext2.h"
#include "reader.h"

#define PERM 0775

void write_jpg(int fd, char *dir_path, f_jpg *jpg);

#endif  // WRITE_H
