#include <stdio.h>

#include "mpi.h"

void print();

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    print();

    MPI_Finalize();

    return 0;
}
