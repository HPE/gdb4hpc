/*
 * Meant for testing the tree-like display of backtraces via the bt command in 
 * gdb4hpc.
 */

#include "mpi.h"
#include <iostream>

void odd() {
    std::cout << "I'm odd!\n";
}

void even() {
    std::cout << "I'm even!\n";
}

int main(int argc, char* argv[]) {
    int ierr = MPI_Init(&argc, &argv);
    if (ierr != MPI_SUCCESS) {
	    std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
    int rank;
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
    int num_ranks;
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }

    // branch execution path in a psuedo-random fashion
    if (rank & 1 == 0) {
        odd();
    } else {
        even();
    }

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
    return 0;
}
