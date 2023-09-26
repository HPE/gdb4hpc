// This binary is used to test gdb4hpc's detection of non-MPI binaries.

#include <stdio.h>

int main() {
    printf("I am not an mpi binary! Sorry!\n");
    return 0;
}
