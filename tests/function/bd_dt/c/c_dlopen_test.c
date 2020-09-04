/*
 © Copyright 2019-2020 Hewlett Packard Enterprise Development LP.
 Unpublished Proprietary Information.
 This unpublished work is protected to trade secret, copyright and other laws.
 Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 no part of this work or its content may be used, reproduced or disclosed
 in any form.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dlfcn.h>

#include "mpi.h"

void *libdlopen_test_handle = NULL;
int (*dlopened_function)(void) = NULL;

int main(int argc, char* argv[]) {
fprintf(stderr, "%s\n", BIN_PATH);
  int myRank, numProcs;

  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  libdlopen_test_handle = dlopen(BIN_PATH "/libdlopen_test.so", RTLD_LAZY);
  if (libdlopen_test_handle == NULL) {
    fprintf(stderr, "dlopen libdlopen_test.so failed\n");
    return -1;
  }
  dlerror();
  dlopened_function = dlsym(libdlopen_test_handle, "dlopened_function");
  if (dlopened_function == NULL) {
    fprintf(stderr, "dlsym dlopened_function failed: %s\n", dlerror());
    return -1;
  }

  int result = dlopened_function();
  fprintf(stderr, "dlopened_function returned %d\n", result);

  if (libdlopen_test_handle != NULL) {
    dlclose(libdlopen_test_handle);
  }

  MPI_Finalize();

  return 0;
}

