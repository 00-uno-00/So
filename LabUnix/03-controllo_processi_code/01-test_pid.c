#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Test process id\n");

  printf("Id del processo: %d\n",getpid());
  printf("Id del processo padre: %d\n",getppid());

  sleep(30);

}
