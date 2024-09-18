#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>

#define NUM_MAX 30000
#define NUM_KIDS 90

int end=0;

void signalHaldler(int sig) {
  switch (sig) {
    case SIGINT:
      printf("CHILD %d - Received: SIGINT\n",getpid());
      end=1;
      break;
    case SIGUSR1:
      printf("CHILD %d - Received: SIGUSR1\n",getpid());
      break;
    case SIGUSR2:
      printf("CHILD %d - Received: SIGUSR2\n",getpid());
      break;
  }
  printf("CHILD %d - ok\n",getpid());
}

int main() {
  int file_pipes[2]; // [0] --> read - [1] --> write

  pid_t fork_result;

  if (pipe(file_pipes) == 0) {
    for (int i=0; i<NUM_KIDS; i++) {
      fork_result = fork();
      switch (fork_result) {
        case -1:
          fprintf(stderr, "%s: %d. Error in fork #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
          exit(EXIT_FAILURE);
        case 0:
          printf("CHILD PID %5d - hello!\n", getpid());
          close(file_pipes[0]); // close read end


          int iter=0;

          struct sigaction sa;

          sigset_t mask_INT_USR1;
          sigemptyset(&mask_INT_USR1);                  /* set an empty mask */
          sigaddset(&mask_INT_USR1, SIGINT);  
          sigaddset(&mask_INT_USR1, SIGUSR1); 

          sigset_t mask_empty;
          sigemptyset(&mask_empty);

          sa.sa_mask = mask_empty;
          sa.sa_handler = signalHaldler;
          sa.sa_flags = 0;
          sigaction(SIGINT, &sa, NULL);
          sigaction(SIGUSR1, &sa, NULL);

          pid_t pid=getpid();

          srand(getpid());
          
          char *buf=malloc(sizeof(int)+sizeof(pid_t));

          sigprocmask(SIG_SETMASK, &mask_INT_USR1, NULL);

          while(!end){
            int n=rand()%NUM_MAX+1;
            printf("CHILD %d - Number %d\n", getpid(), n);

            memcpy(buf,&pid,sizeof(pid_t));
            memcpy(buf+sizeof(pid_t),&n,sizeof(int));

            //printf("CHILD %d - Data ready\n", getpid());

            int rst=write(file_pipes[1], buf, sizeof(int)+sizeof(pid_t));
            if (rst==-1) {
              fprintf(stderr, "CHILD %d  - %s: %d. Error in writing pipe #%03d: %s\n", getpid(), __FILE__, __LINE__, errno, strerror(errno));
            }
            //printf("CHILD %d - Wrote %d bytes\n", getpid(), rst);

            //printf("CHILD %d - %d - pausing...\n",getpid(),iter);

            //sigprocmask(SIG_SETMASK, &mask_empty, NULL);
            //pause();
            sigsuspend(&mask_empty);
            //sigprocmask(SIG_SETMASK, &mask_INT_USR1, NULL);
            //printf("CHILD %d - %d - restart...\n",getpid(),iter);
            iter++;
          }
          
          close(file_pipes[1]); //close write end

          printf("CHILD %d - exit\n",getpid());
          exit(EXIT_SUCCESS);
          break;
        default:
          break;
          
      }
    }
    printf("PARENT PID %5d\n", getpid());
    close(file_pipes[1]); // close write end

    int remainingChildred=NUM_KIDS;

    int i=0;

    //sleep(5);

    while (remainingChildred>0) {

      int minValue=INT_MAX;
      pid_t minPid=-1;

      pid_t remainingPid[remainingChildred];

      for (int i=0;i<remainingChildred;i++) {
        int value;
        pid_t cPid;

        //printf("PARENT - (%d) reading pid...\n",i);
        int rst=read(file_pipes[0],&cPid,sizeof(pid_t));
        if (rst==-1) {
            fprintf(stderr, "%s: %d. Error in reading pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        }

        //printf("PARENT - (%d) reading value...\n",i);
        rst=read(file_pipes[0],&value,sizeof(int));
        if (rst==-1) {
            fprintf(stderr, "%s: %d. Error in reading pipe #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        }

        printf("PARENT - (%d) From %d - Value %d\n",i,cPid,value);

        remainingPid[i]=cPid;

        if (value<minValue) {
          minValue=value;
          minPid=cPid;
        }
      }

      printf("PARENT - Min value %d from %d\n",minValue,minPid);

      for (int i=0;i<remainingChildred;i++) {
        if (remainingPid[i]==minPid) {
          kill(remainingPid[i], SIGINT);
          printf("PARENT - Sent SIGINT to %d\n",remainingPid[i]);
        }
        else {
          kill(remainingPid[i], SIGUSR1);
          printf("PARENT - Sent SIGUSR1 to %d\n",remainingPid[i]);
        }
      }

      remainingChildred--;
      //sleep(1);
    }

    close(file_pipes[0]); //close read end

    printf("PARENT: exit\n");
    exit(EXIT_SUCCESS);
  } else {
    fprintf(stderr, "%s: %d. Error in pipe creation #%03d: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}
