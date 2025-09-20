#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
pid_t pid;
int counter = 0;

void handler1(int sig) {
    counter++;
    printf("counter = %d\n", counter);
    fflush(stdout); // flush immediately
    kill(pid, SIGUSR1); // send signal to child
}

void handler2(int sig) {
    counter += 3;
    printf("counter = %d\n", counter);
    exit(0);
}

int main() {
    pid_t p;
    int status;

    signal(SIGUSR1, handler1);

    if ((pid = fork()) == 0) {
        // child process
        signal(SIGUSR1, handler2);
        kill(getppid(), SIGUSR1); // notify parent
        while (1) ; // loop forever until signal handler exits
    }

    // parent waits for child termination
    if ((p = wait(&status)) > 0) {
        counter += 4;
        printf("counter = %d\n", counter);
    }

    return 0;
}
