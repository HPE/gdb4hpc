#include "mpi.h"
#include <vector>
#include <array>
#include <iostream>

static double getValue1(int i)
{
    return i; //i%17 - (i/13) * 2.5 + (i%3) * 0.5;
}

static double getValue2(int i, int j)
{
    return getValue1(100*i+j);
}

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

    const size_t size = 120;
    const size_t sizeI = 12;
    const size_t sizeJ = 10;
    const size_t cycle = 5;

    const size_t sizeIT = sizeJ;
    const size_t sizeJT = sizeI;

    size_t sliceN = size / num_ranks;
    size_t sliceJ = sizeJ / num_ranks;

    std::vector<double> block(sliceN);
    std::vector<double> cyclic(sliceN);
    std::vector<double> blockCyclic(sliceN);

    std::vector<double> block2(sliceN);
    std::vector<double> cyclic2(sliceN);
    std::vector<double> blockCyclic2(sliceN);

    std::vector<float> block2T(sliceN);

    int bcSizeI = 24;
    int bcSizeJ = 5;
    int cycle2 = 2;

    for (size_t i=0; i<sliceN; ++i) {
        block[i] = getValue1(sliceN * rank + i);
        cyclic[i] = getValue1(rank + i * num_ranks);
        blockCyclic[i] = getValue1(rank*cycle + (i/cycle)*num_ranks*cycle + i%cycle);

        int locI = i/sizeJ;
        int locJ = i%sliceJ;
        block2[i] = getValue2(rank * (sliceN/sizeJ) + i/sizeJ, i%sizeJ);
        cyclic2[i] = getValue2(rank + num_ranks * (i/sizeJ), i%sizeJ);

        int bcI = i / bcSizeJ;
        int bcJ = i % bcSizeJ;
        blockCyclic2[i] = getValue2(rank*cycle2 + (bcI/cycle2)*num_ranks*cycle2 + bcI%cycle2, bcJ); 

        block2T[i] = getValue2(i / (sizeJT/num_ranks), rank * (sizeJT/num_ranks) + i%(sizeJT/num_ranks));
    }

    const size_t modI = sliceN/2;
    const double modVal = -9999.9;
             
    if (application) {
        block[modI] = modVal;
        cyclic[modI] = modVal;
        blockCyclic[modI] = modVal;
        block2[modI] = modVal;
        cyclic2[modI] = modVal;
        blockCyclic2[modI] = modVal;
        block2T[modI] = modVal;
    }

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
    return 0;
}
