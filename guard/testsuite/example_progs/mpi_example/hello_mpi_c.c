/*********************************************************************************\
 * Very simple hello-world using C and MPI
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/


#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char* argv[]) {
  int myRank, numProcs;
  int source, dest=0, tag=0;
  char message[100];
  MPI_Status status;

  MPI_Init(&argc,&argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

  if (myRank != 0) {
    sprintf(message,"Hello World! from process %d", myRank);
    MPI_Send(message, strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
  } else {
    printf("Hello World! from process %d\n", myRank);

    for(source=1; source<numProcs; source++) {
      MPI_Recv(message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
      printf("%s\n", message);
    }
  }

  MPI_Finalize();

  return 0;
}

