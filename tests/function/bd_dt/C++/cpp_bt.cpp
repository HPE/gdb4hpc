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
void even(int rank);
void odd(int rank);

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
    if ((rank & 1) == 0) {
        even(rank);
    } else {
        odd(rank);
    }
}

void rare_branch(int rank) {
    step_three(rank);
}

void merge(int rank) {
    // serves as an easy point to set a single breakboint
}

void step_one(int rank) {
    step_two(rank);
}

void step_two(int rank) {
    if (rank == 3 || rank == 4) {
        rare_branch(rank);
    }
    step_three(rank);
}

void step_three(int rank) {
    merge(rank);
}

void even(int rank) {
    std::cout << "I'm even!\n";
    step_one(rank);
}

void odd(int rank) {
    std::cout << "I'm odd!\n";
    step_one(rank);
}
