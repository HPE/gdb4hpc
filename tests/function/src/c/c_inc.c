// designed to test checkpoints
#include "mpi.h"

void do_inc(unsigned int *inc, unsigned int by) {
    *inc += by;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    unsigned int inc = 0;

    do_inc(&inc, 1);
    do_inc(&inc, 2);
    do_inc(&inc, 3);
    do_inc(&inc, 4);
    do_inc(&inc, 5);

    MPI_Finalize();
    return 0;
}