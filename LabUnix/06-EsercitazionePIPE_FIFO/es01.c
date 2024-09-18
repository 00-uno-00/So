#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


int main() {
  int file_pipes[2]; // [0] --> read - [1] --> write

  pid_t fork_result;

  if (pipe(file_pipes) == 0) {
    fork_result = fork();
    switch (fork_result) {
      case -1:
        fprintf(stderr, "%s: %d. Error in fork #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        exit(EXIT_FAILURE);
      case 0:
        printf("CHILD PID %5d\n", getpid());
        close(file_pipes[1]); // close write end

        sleep(2); // do some reading

        close(file_pipes[0]); // close read end

        printf("CHILD: exit\n");
        exit(EXIT_SUCCESS);
      default:
        printf("PARENT PID %5d\n", getpid());
        close(file_pipes[0]); // close read end

        sleep(2); // do some writing

        close(file_pipes[1]); // close write end

        printf("PARENT: exit\n");
        exit(EXIT_SUCCESS);
    }
  } else {
    fprintf(stderr, "%s: %d. Error in pipe creation #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
