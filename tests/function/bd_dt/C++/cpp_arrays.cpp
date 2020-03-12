#include "mpi.h"
#include <vector>
#include <array>
#include <iostream>

int main(int argc, char* argv[])
{
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

    int application = 0;
    if (argc > 1)
        application = atoi(argv[1]);

    int size = 20;
    if (argc > 2)
        size = atoi(argv[2]);

    int array2d[40][20];
    for (int i=0; i<40; ++i) {
        for (int j=0; j<20; ++j) {
            array2d[i][j] = 2*i - j;
        }
    }

    int *distributedArray1 = new int[size];
    for (int i=0; i<size; ++i)
        distributedArray1[i] = 4*i;

    int localSize = 6 * size / num_ranks;
    int idx0 = rank * localSize * 2;
    int* localArray1 = new int[localSize];
    for (int i=0; i<localSize; ++i) {
        localArray1[i] = idx0 + 2 * i;
    }

    std::array<int,10> std_array;
    for (int i=0; i<10; ++i)
        std_array[i] = 5 * i;

    std::vector<long> distributedVec1;
    for (int i=0; i<size; ++i)
        distributedVec1.push_back(i);

    std::vector<int> localVec(localSize);
    for (int i=0; i<localSize; ++i) {
        localVec[i] = idx0 + 2 * i;
    }

    if (application) {
        localArray1[localSize-1] = -100;
        distributedArray1[size-1] = -100;
        array2d[4][5] = -100;
        distributedVec1[size/2] = -100;
        std_array[4] = -100;
        localVec[localSize-2] =100;
    }

    delete[] distributedArray1;
    delete[] localArray1;

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_init() failed: " << ierr;
    }
    return 0;
}
