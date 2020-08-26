#include "mpi.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    
    char one_d[] = "Hello world!\n";
    unsigned char one_d_u[] = "Numbers"; // will be printed as decimal numbers
    signed char one_d_s[] = "Hello world! (signed)\t\n"; // treated the same as char
    char two_d[4][16] = {"Hello", "World", "Once", "Again!\n"};
    unsigned char two_d_u[4][4] = {"abc", "123", "xyz", "098"};
    
    MPI_Finalize();
    return 0;
}