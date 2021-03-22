#include "mpi.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int *p = 0;
    // oops!
    *p = 2048;

    MPI_Finalize();

    return 0;
}
