#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cmath>

#include "memoryManager.hpp"
#include "RAJA/RAJA.hpp"

#include "mpi.h"

int main(int argc, char* argv[]) {
    int my_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Initialize
    constexpr int Nx = 3;
    constexpr int Ny = 10;
    constexpr int Nz = 2;
    constexpr int Ntot  = Nx*Ny*Nz;
    int aref[Ntot];
    int a[Ntot];
    int aref_2D[Nx][Ny*Nz];
    int aref_3D[Nx][Ny][Nz];

    for (int i = 0; i < Ntot; ++i) {
        aref[i] = i;
        aref_2D[i/(Ny*Nz)][i%(Ny*Nz)] = i;
        aref_3D[i/(Ny*Nz)][(i/Nz)%Ny][i%Nz] = i;
    }

    for (int i = 0; i < Ntot; i++) {
        for (int j = 0; j < my_rank; j++) {
            aref[i] *= 10;
            aref_2D[i/(Ny*Nz)][i%(Ny*Nz)] *= 10;
            aref_3D[i/(Ny*Nz)][(i/Nz)%Ny][i%Nz] *= 10;
        }
    }

    // 1D
    std::memset(a, 0, Ntot * sizeof(int));
    RAJA::View< int, RAJA::Layout<1, int> > view_1D(a, Ntot);
    for (int i = 0; i < Ntot; ++i) {
        view_1D(i) = aref[i];
    }

    // 1D offset
    std::memset(a, 0, Ntot * sizeof(int));
    int imin = -30;
    int imax = Ntot - 30;
    RAJA::OffsetLayout<1, int> offlayout_1D =
        RAJA::make_offset_layout<1, int>( {{imin}}, {{imax}} );
    RAJA::View< int, RAJA::OffsetLayout<1, int> > view_1Doff(a, offlayout_1D);
    for (int i = imin; i < imax; ++i) {
        view_1Doff(i) = aref[i+30];
    }

    // 2D
    std::memset(a, 0, Ntot * sizeof(int));
    RAJA::View< int, RAJA::Layout<2, int> > view_2D(a, Nx, Ny*Nz);
    int iter{0};
    for (int i = 0; i < Nx; ++i) {
        for (int j = 0; j < Ny*Nz; ++j) {
          view_2D(i, j) = aref[iter];
          ++iter;
        }
    }

    // 3D
    std::memset(a, 0, Ntot * sizeof(int));
    RAJA::View< int, RAJA::Layout<3, int> > view_3D(a, Nx, Ny, Nz);
    iter = 0;
    for (int i = 0; i < Nx; ++i) {
        for (int j = 0; j < Ny; ++j) {
            for (int k = 0; k < Nz; ++k) {
                view_3D(i, j, k) = aref[iter];
                ++iter;
            }
        }
    }

    MPI_Finalize();
}
