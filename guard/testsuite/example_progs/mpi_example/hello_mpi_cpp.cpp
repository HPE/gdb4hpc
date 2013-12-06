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

