#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE    100

int main(int argc, char * argv[])
{
  int fifo_fd;

  char readbuf[BUF_SIZE];

  ssize_t num_bytes;

  /* Create the FIFO if it does not exist */
  if (mkfifo(argv[1], S_IRUSR | S_IWUSR)==-1) {
      fprintf(stderr, "%s: %d. Error in creating fifo #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
      exit(EXIT_FAILURE);
  }

  if ((fifo_fd = open(argv[1], O_RDONLY))== -1) {
      fprintf(stderr, "%s: %d. Error in opening fifo #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
      exit(EXIT_FAILURE);
  }

  while (num_bytes = read(fifo_fd, readbuf, BUF_SIZE)) {
    if (num_bytes==-1) {
      fprintf(stderr, "%s: %d Error in reading fifo #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    }
    printf("READ %ld bytes: %s\n", num_bytes, readbuf);
    
    memset(readbuf, '\0', sizeof(readbuf));
  }

  close(fifo_fd);
}
