#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>

int main() {
    pid_t p;
    int i, count;
    int status;
    struct sched_param param;

    for (i = 0; i < 3; i++) {
        p = fork();
        if (p == 0) {
            // Priority
            param.sched_priority = 10 + i;
            // SCHED_RR, SCHED_OTHER, SCHED_FIFO
            sched_setscheduler(getpid(), SCHED_OTHER, &param);
            count = 0;
            while (count < 10) {
                printf("i = %d, count = %d.\n", i, count++);
                sleep(1);
            }
            exit(i);
        }
    }

    while (1) {
        p = wait(&status);
        if (p <= 0) {
            break;
        }
        printf("Child process %d exited.\n", p);
    }
}
