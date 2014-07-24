#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define TAR_MODE S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH

int
main(int argc, char *argv[])
{
  int 	src_fd;
  int   des_fd;
  int 	src_length;
  int 	cur_length;
  int	read_length;
  int	write_length;
  int	copy_buf_size;
  struct stat stat_buf;

  if (argc != 3)
    printf("Usage: %s <src-path> <des-path>\n", argv[0]), exit(1);

  if ((src_fd = open(argv[1], O_RDONLY)) < 0)
    printf("can't open %s: Permission denied\n", argv[1]), exit(1);

  if (fstat(src_fd, &stat_buf) < 0)
    printf("stat error\n"), exit(1);

  src_length = stat_buf.st_size;
  copy_buf_size = stat_buf.st_blksize;

  if ((des_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, TAR_MODE)) < 0)
    printf("can't create or open %s: Permission denied\n", argv[2]), exit(1);

  char buf[copy_buf_size];
  char *p = buf;
  while (src_length > 0)
    {
      cur_length = read(src_fd, p, copy_buf_size);
      read_length = cur_length;
      while (cur_length > 0)
        {
          write_length = write(des_fd, p, cur_length);
          cur_length -= write_length;
          p += write_length;
        }
      p = buf;
      src_length -= read_length;
    }

  close(src_fd);
  close(des_fd);
  exit(0);
}
