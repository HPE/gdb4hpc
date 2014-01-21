/*********************************************************************************\
 * hello_mpi_cpp.cpp
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


#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

#define BUFFER_SIZE 100

int main(int argc, char** argv) {

  MPI::Init(argc, argv);

  int rank = MPI::COMM_WORLD.Get_rank();
  int size = MPI::COMM_WORLD.Get_size();

  char message[BUFFER_SIZE];
  sprintf(message, "Hello World! from process %d", rank);

  int messageSize= strlen(message)+1;
  int to= 0;
  int tag= 0;

  if (rank == 0) {
    cout << message << endl;

    MPI::Status status;
    for (int source=1; source < size; source++) { 
      MPI::COMM_WORLD.Recv(message, BUFFER_SIZE, MPI::CHAR, source, tag, status);
      cout << message << endl;
    }
  } else {
    MPI::COMM_WORLD.Send(message, messageSize, MPI::CHAR, to, tag);
  }

  MPI::Finalize();

  return 0;
}

