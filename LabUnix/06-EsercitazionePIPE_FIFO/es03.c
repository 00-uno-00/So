#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define N 10

int main() {
  int file_pipes[2]; // [0] --> read - [1] --> write

  pid_t fork_result;

  ssize_t rst=0;

  if (pipe(file_pipes) == 0) {
    fork_result = fork();
    switch (fork_result) {
      case -1:
        fprintf(stderr, "%s: %d. Error in fork #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        exit(EXIT_FAILURE);
      case 0:
        printf("CHILD PID %5d\n", getpid());
        close(file_pipes[1]); // close write end

        int data_processed;
        int data;


        while((rst=read(file_pipes[0], &data, sizeof(int)))) {
          if (rst==-1) {
            fprintf(stderr, "%s: %d. Error in reading pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);
          }

          printf("Read %ld bytes - value:  %d\n", rst, data);
          printf("Waiting...\n");
          sleep(1);
        }

        close(file_pipes[0]);

        printf("CHILD: exit\n");
        exit(EXIT_SUCCESS);
      default:
        printf("PARENT PID %5d\n", getpid());
        close(file_pipes[0]); // close read end

        int i=0;

        for(i=0; i<N; i++) {
          if ((rst=write(file_pipes[1], &i, sizeof(int)))==-1) {
            fprintf(stderr, "%s: %d. Error in writing pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            exit(EXIT_FAILURE);
          }

          printf("Wrote %ld bytes - value: %d\n", rst, i);
          //sleep(1);
	      }

        sleep(20);

        close(file_pipes[1]);

        printf("PARENT: exit\n");
        exit(EXIT_SUCCESS);
    }
  } else {
    fprintf(stderr, "%s: %d. Error in pipe creation #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
