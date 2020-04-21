HIPCC = hipcc

CXXFLAGS   = -g -O0
ARCH_FLAGS = --amdgpu-target=gfx906
INCLUDES   = -I/opt/cray/pe/cray-mvapich2_nogpu/2.3.1/./infiniband/cray/9.0/include
LD_INCS    = -L/opt/cray/pe/cray-mvapich2_nogpu/2.3.1/./infiniband/cray/9.0/lib -lmpich \
			 -L${GCC_X86_64}/lib64 -lstdc++
PROG       = square_hip_mpi
SRCS       = $(wildcard *.cpp)

all: $(PROG)

$(PROG): $(SRCS)
	HIPCC_VERBOSE=1 HIP_PLATFORM=hcc \
	$(HIPCC) $(CXXFLAGS) $(ARCH_FLAGS) $(INCLUDES) $(LD_INCS) $^ -o $(PROG)

clean:
	rm -f $(PROG)

