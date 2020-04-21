#include "hip/hip_runtime.h"
/*
 * Copyright 1993-2015 NVIDIA Corporation.  All rights reserved.
 *
 * Please refer to the NVIDIA end user license agreement (EULA) associated
 * with this source code for terms and conditions that govern your use of
 * this software. Any use, reproduction, disclosure, or distribution of
 * this software and related documentation outside the terms of the EULA
 * is strictly prohibited.
 *
 */


/* Simple example demonstrating how to use MPI with HIP
*
*  Generate some random numbers on one node.
*  Dispatch them to all nodes.
*  Compute their square root on each node's GPU.
*  Compute the average of the results using MPI.
*
*  gpu_code.cpp: GPU part
*/

#include <iostream>
using std::cerr;
using std::endl;

#include "simpleMPI.h"

// Error handling macro
#define HIP_CHECK(call) \
    if((call) != hipSuccess) { \
        hipError_t err = hipGetLastError(); \
        cerr << "HIP error calling \""#call"\", code is " << err << endl; \
        my_abort(err); }


// Device code
// Very simple GPU Kernel that computes square roots of input numbers
__global__ void simpleMPIKernel(float *input, float *output)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    output[tid] = sqrt(input[tid]);
}


// Initialize an array with random data (between 0 and 1)
void initData(float *data, int dataSize)
{
    for (int i = 0; i < dataSize; i++)
    {
        data[i] = (float)rand() / (float)RAND_MAX;
    }
}

// HIP computation on each node
// No MPI here, only HIP
void computeGPU(float *hostData, int blockSize, int gridSize)
{
    int dataSize = blockSize * gridSize;

    // Allocate data on GPU memory
    float *deviceInputData = NULL;
    HIP_CHECK(hipMalloc((void **)&deviceInputData, dataSize * sizeof(float)));

    float *deviceOutputData = NULL;
    HIP_CHECK(hipMalloc((void **)&deviceOutputData, dataSize * sizeof(float)));

    // Copy to GPU memory
    HIP_CHECK(hipMemcpy(deviceInputData, hostData, dataSize * sizeof(float), hipMemcpyHostToDevice));

    // Run kernel
    hipLaunchKernelGGL(simpleMPIKernel, dim3(gridSize), dim3(blockSize), 0, 0, deviceInputData, deviceOutputData);

    // Copy data back to CPU memory
    HIP_CHECK(hipMemcpy(hostData, deviceOutputData, dataSize *sizeof(float), hipMemcpyDeviceToHost));

    // Free GPU memory
    HIP_CHECK(hipFree(deviceInputData));
    HIP_CHECK(hipFree(deviceOutputData));
}

float sum(float *data, int size)
{
    float accum = 0.f;

    for (int i = 0; i < size; i++)
    {
        accum += data[i];
    }

    return accum;
}
