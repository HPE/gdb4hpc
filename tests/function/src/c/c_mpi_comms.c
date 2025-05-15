// Copyright 2025 Hewlett Packard Enterprise Development LP.
#include "mpi.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int color = world_rank / 4; // Determine color based on row

    // Split the communicator based on the color and use the
    // original rank for ordering
    MPI_Comm row_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm);
    MPI_Comm_set_name(row_comm, "ROW");

    int row_rank, row_size;
    MPI_Comm_rank(row_comm, &row_rank);
    MPI_Comm_size(row_comm, &row_size);

    printf("WORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d\n",
            world_rank, world_size, row_rank, row_size);

    if (row_rank % 2 == 0) {
        MPI_Request request;
        char message[128];
        sprintf(message,"Hello World! from process %d", row_rank);

        MPI_Isend(message, strlen(message)+1, MPI_CHAR, row_rank + 1, 7, row_comm, &request);
        printf("%d: Sent async message to %d: %s\n", world_rank, row_rank + 1, message);

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("Called MPI_Wait\n");
    } else {
        MPI_Request request;
        char message[128];

        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        
        for (volatile int i = 0; i<10; i++);
        
        MPI_Irecv(message, 100, MPI_CHAR, row_rank - 1, 7, row_comm, &request);
        printf("Called Irecv\n");

        MPI_Wait(&request, MPI_STATUS_IGNORE);
        printf("%d: Got message from %d: %s\n", world_rank, row_rank - 1, message);
    }

    MPI_Comm_free(&row_comm);

    MPI_Finalize();

    return 0;
}
