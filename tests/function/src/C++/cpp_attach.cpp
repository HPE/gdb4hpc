#include <unistd.h>
#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    // ./cpp_attach <number of reps>
    int reps = atoi(argv[1]);
    int i = 0;

    for (i = 0; argc == 2 && i < reps; i++) {
        // wait for a debugger to attach
        // in a debugger, change argc to break the loop
        sleep(1);
    }

    // std::cout << "done after " << i << " reps." << std::endl;

    MPI_Finalize();

    return 0;
}