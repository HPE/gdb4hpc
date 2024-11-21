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

    // also update py_main_fns below if needed.
    // see "Getting the GIL after Launch..." in python.md
    "Py_Initialize", // python 3.6
    "Py_InitializeEx", // 3.6
    "PyImport_GetImporter", // 3.7
    "Py_InitializeFromConfig", // 3.8+
};

// we handle python entry points like we handle MPI entry points. we
// break in the python init function (listed in parallel_entry_pt_strs), then
// run a finish command. the functions in py_main_fns are the functions that we
// will end up in after the finish command. they are used to handle the
// special case of running pybe commands right after hitting the entry
// breakpoint.
// see "Getting the GIL after Launch..." in python.md
constexpr auto py_main_fns = std::array{
    "Py_Main",
    "pymain_init",
    "pymain_get_importer",
};
