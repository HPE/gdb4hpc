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
        std::cerr << "Error: MPI_init() failed: " << ierr << "\n"; 
    }
    int num_ranks;
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);
    if (ierr != MPI_SUCCESS) {
        std::cerr << "Error: MPI_init() failed: " << ierr << "\n";
    }

    int application = 0;
    if (argc > 1)
        application = atoi(argv[1]);

    if (num_ranks != 6) {
        std::cerr << "Test Error: expected 6 ranks\n";
        MPI_Finalize();
        return -1;
    }

    const size_t size = 120;
    const size_t sizeI = 12;
    const size_t sizeJ = 10;
    const size_t cycle = 5;

    size_t sliceN = size / num_ranks;

    // block12 and block21 both have decompostion
    // 3,2 - but block12 has dim_order 1,2
    // and block21 has dim_order 2,1
    std::vector<int> block12(sliceN);
    std::vector<int> block21(sliceN);

    const size_t sliceI = sizeI / 3;
    const size_t sliceJ = sizeJ / 2;

    int chunkI_12 = rank % 3;
    int chunkJ_12 = rank / 3;

    int chunkI_21 = rank / 2;
    int chunkJ_21 = rank % 2;

    for (int idx=0; idx<sliceN; ++idx) {
        int i = idx / sliceJ;
        int j = idx % sliceJ;
        
        block12[idx] = getValue2(chunkI_12 * sliceI + i, chunkJ_12 * sliceJ + j);
        block21[idx] = getValue2(chunkI_21 * sliceI + i, chunkJ_21 * sliceJ + j);
    }

    const size_t modI = sliceN/2;
    const double modVal = -9999.9;
             
    if (application) {
        block12[modI] = modVal;
        block21[modI] = modVal;
    }

    ierr = MPI_Finalize();
    if (ierr != MPI_SUCCESS) {
        std::cout << stderr << "Error: MPI_Finalize() failed: " << ierr;
    }
    return 0;
}
