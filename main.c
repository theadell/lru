#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
int main(int argc, char **argv) {

  const char *program = *argv++;
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <input.txt> \n", program);
    fprintf(stderr, "Error: Input file was not provided\n");
    exit(1);
  }
  const char *file_path = *argv++;

  int fd = open(file_path, O_RDONLY);
  if (fd < 0) {
    fprintf(stderr, "ERROR: could not open file %s: %s\n", file_path,
            strerror(errno));
    exit(1);
  }

  struct stat statbuf = {0};
  if (fstat(fd, &statbuf) < 0) {

    fprintf(stderr, "ERROR: could not get size of file %s: %s\n", file_path,
            strerror(errno));
    exit(1);
  }

  size_t content_size = statbuf.st_size;

  char *content_data = mmap(NULL, content_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (content_data == NULL) {
    fprintf(stderr, "ERRPR: Couldn't memory map file %s: %s\n", file_path,
            strerror(errno));
    exit(1);
  }
  fwrite(content_data, content_size, 1, stdout);

  munmap(content_data, content_size);
  close(fd);
  return 0;
}
