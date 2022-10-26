/*
 * meant to stress test the bt tree view
 */

#include "mpi.h"
#include <iostream>
#include <cmath>

void branch(int rank, int num_ranks, int level);
void loop(int times);

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

    branch(rank, num_ranks, 0);

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
    return 0;
}

void branch(int rank, int num_ranks, int level) {
    if (level >= int(std::log2(num_ranks))) {
        loop(4);
    } else {
        if (((rank >> level) & 1) == 1) {
            branch(rank, num_ranks, level + 1);
        } else {
            branch(rank, num_ranks, level + 1);
        }
    }
}

void loop(int times) {
    if (times > 0) {
        loop(times - 1);
    } else {
        // all done - breakpoint here then backtrace
    }
}
