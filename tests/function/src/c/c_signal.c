#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "mpi.h"

int exit_loop = 0;

void sighandler(int signum) {
    if (signum == SIGUSR1) {
        fprintf(stderr, "Received SIGUSR1\n");
        exit_loop = 1;
    }
    if (signum == SIGUSR2) {
        fprintf(stderr, "Received SIGUSR2\n");
        signal(SIGUSR2, sighandler);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    signal(SIGUSR1, sighandler);
    signal(SIGUSR2, sighandler);

	fprintf(stderr, "Registered signal handlers\n");

    while (!exit_loop) { sleep(1); }

    fprintf(stderr, "Loop exited\n");

    MPI_Finalize();

    return 0;
}
