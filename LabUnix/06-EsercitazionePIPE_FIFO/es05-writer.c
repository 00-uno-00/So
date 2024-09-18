#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE    100

int main(int argc, char * argv[])
{
  int fifo_fd, str_len;
  char my_msg[BUF_SIZE];

  ssize_t data_processed;

  // Open FIFO in write mode
  while ((fifo_fd = open(argv[1], O_WRONLY))==-1) {
    fprintf(stderr, "%s: %d. Error in open() #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    sleep(1);
  }

  // Write message to FIFO
  sprintf(my_msg,"Primo messaggio");
  data_processed=write(fifo_fd, my_msg, strlen(my_msg));
  printf("Wrote %ld bytes\n",data_processed);
  sleep(1);

  sprintf(my_msg,"Secondo messaggio");
  data_processed=write(fifo_fd, my_msg, strlen(my_msg));
  printf("Wrote %ld bytes\n",data_processed);
  sleep(1);

  sprintf(my_msg,"Terzo messaggio");
  data_processed=write(fifo_fd, my_msg, strlen(my_msg));
  printf("Wrote %ld bytes\n",data_processed);
  sleep(1);

  close(fifo_fd);

  return(0);
}
