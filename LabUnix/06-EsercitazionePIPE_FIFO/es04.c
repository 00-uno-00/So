#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define N 10
#define BUFFER_SIZE 100

int main() {
  int file_pipes[2]; // [0] --> read - [1] --> write

  char *message="Hello brother!";

  char mode='R';

  pid_t fork_result;

  ssize_t rst=0;

  if (pipe(file_pipes) == 0) {
    int i=0;

    for (;i<2;i++) {

      if (i==1) {
        mode='W';
      }

      fork_result = fork();
      switch (fork_result) {
        case -1:
          fprintf(stderr, "%s: %d. Error in fork #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
          exit(EXIT_FAILURE);
        case 0:
          printf("CHILD PID %5d - MODE: %c\n", getpid(), mode);

          if (mode=='R') {
            // read MODE
            close(file_pipes[1]); // close write end

            int data_processed;
            char data[BUFFER_SIZE]="";

            if((rst=read(file_pipes[0], &data, BUFFER_SIZE))==-1) {
              fprintf(stderr, "%s: %d. Error in reading pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            } else {
              printf(" %5d Read %ld bytes - value:  %s\n", getpid(), rst, data);
            }

            close(file_pipes[0]);
          } else {
            //write MODE
            close(file_pipes[0]); // close read end

            if ((rst=write(file_pipes[1], message, strlen(message)))==-1) {
              fprintf(stderr, "%s: %d. Error in writing pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            } else {
              printf("%5d Wrote %ld bytes - value: %s\n",getpid(), rst, message);
            }

            close(file_pipes[1]);
          }

          printf("CHILD %5d: exit\n",getpid());
          exit(EXIT_SUCCESS);
        default:
          printf("PARENT PID %5d\n", getpid());
      }
    }

    printf("PARENT: exit\n");
    exit(EXIT_SUCCESS);

  } else {
    fprintf(stderr, "%s: %d. Error in pipe creation #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
