/*
 * Meant for testing the tree-like display of backtraces via the bt command in 
 * gdb4hpc.
 */

#include "mpi.h"
#include <iostream>

void branch(int rank);
void rare_branch(int rank);
void merge(int rank);
void step_one(int rank);
void step_two(int rank);
void step_three(int rank);
void left(int rank);
void right(int rank);
void loop(int rank, int num);

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

    branch(rank);

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
    return 0;
}

void branch(int rank) {
    // branch execution path in a psuedo-random fashion
    if (rank < 5) {
        left(rank);
    } else {
        right(rank);
    }
}

void rare_branch(int rank) {
    step_three(rank);
}

void merge(int rank) {
    int i=0; // serves as an easy point to set a single breakboint
}

void step_one(int rank) {
    step_two(rank);
}

void step_two(int rank) {
    if (rank == 0 || rank == 5) {
        rare_branch(rank);
    }
    step_three(rank);
}

void step_three(int rank) {
    merge(rank);
}

void left(int rank) {
    loop(rank, 0);
}

void right(int rank) {
    step_one(rank);
}

void loop(int rank, int num) {
    if (num > 3) {
        step_one(rank);
    } else {
        loop(rank, num+1);
    }
}
