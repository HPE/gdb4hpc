// Copyright 2024 Hewlett Packard Enterprise Development LP.

#pragma once

// List of known entry point breakpoints.   
static const auto parallel_entry_pt_strs = std::array
{
    "*(&MPI_Init)",
    "athos_mpi_init",
    "MPI_INIT_THREAD__",
    "MPI_INIT__",
    "mpi_init__",

    "mpi_init_thread__",
    "MPI_INIT_THREAD_",
    "MPI_INIT_",
    "mpi_init_thread_",
    "mpi_init_",

    "MPI::Init",
    "*&(MPI_Init_thread)",
    "MPI_Init()",
    "MPI::Init(int&)",
    "MPI_Init",

    "PMI2_Init",
    "*&(PMPI_Init)",
    "PMPI_Init",
    "*&(start_pes)",
    "*&(shmem_init)",

    "Py_Initialize", // python 3.6
    "Py_InitializeEx", // 3.6
    "Py_InitializeFromConfig", // 3.8+
};

