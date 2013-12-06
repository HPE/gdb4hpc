CC = cc
CXX = CC
FC = ftn

CFLAGS = $(OPTFLAGS) -g
FFLAGS = $(OPTFLAGS) -g

all: hello_mpi_c hello_mpi_f hello_mpi_cpp

hello_mpi_c: hello_mpi.c
	$(CC) $(CFLAGS) -o hello_mpi_c hello_mpi.c

hello_mpi_f: hello_mpi.f90
	$(FC) $(FFLAGS) -o hello_mpi_f hello_mpi.f90

hello_mpi_cpp: hello_mpi.cpp
	$(CXX) $(CFLAGS) -o hello_mpi_cpp hello_mpi.cpp

clean:
	rm hello_mpi_*

