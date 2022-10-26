// designed to test stuff related to exits and exit breakpoints

#include "mpi.h"

void foo() {
    int xyz = 0;
    xyz += 1;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int myRank;

    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // finalize is a sync point; ranks will block here until all ranks make it.
    // to get ranks to exit without other ranks we need to finalize before exiting.
    MPI_Finalize();

    // even ranks call foo(), odd ranks exit
    if ((myRank & 1) == 0) {
        foo();
    }

    return 0;
}