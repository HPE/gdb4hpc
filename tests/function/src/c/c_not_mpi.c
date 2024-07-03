// This binary is used to test gdb4hpc's detection of non-MPI binaries.

#include <stdio.h>

static void print_argc(int argc) {
    printf("%d arguments\n", argc);
}

int main(int argc, char** argv) {

    printf("I am not an mpi binary! Sorry!\n");

    print_argc(argc);

    return 0;
}
