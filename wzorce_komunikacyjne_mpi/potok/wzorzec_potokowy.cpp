#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define VECTOR_SIZE 1000000
#define CHUNK_SIZE 1000

void mpi_cleanup() {
    MPI_Finalize();
}

int verify_data(const int *received, int offset, int size) {
    for (int i = 0; i < size; ++i) {
        if (received[i] != offset + i) {
            return 0; 
        }
    }
    return 1; 
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    atexit(mpi_cleanup);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    MPI_Comm cartesian_comm;
    int dimensions[] = {world_size};
    int periods[] = {0}; 
    MPI_Cart_create(MPI_COMM_WORLD, 1, dimensions, periods, 1, &cartesian_comm);

    int cartesian_rank;
    MPI_Comm_rank(cartesian_comm, &cartesian_rank);

    int prev_rank, next_rank;
    MPI_Cart_shift(cartesian_comm, 0, 1, &prev_rank, &next_rank);

    int *vector = NULL;
    if (cartesian_rank == 0) {
        vector = (int *)malloc(VECTOR_SIZE * sizeof(int));
        for (int i = 0; i < VECTOR_SIZE; ++i) {
            vector[i] = i;
        }
    }

    for (int i = 0; i < VECTOR_SIZE / CHUNK_SIZE; ++i) {
        int chunk[CHUNK_SIZE];

        if (cartesian_rank == 0) {
            int offset = i * CHUNK_SIZE;
            memcpy(chunk, &vector[offset], CHUNK_SIZE * sizeof(int));
            MPI_Send(chunk, CHUNK_SIZE, MPI_INT, next_rank, 0, cartesian_comm);
        } else {
            MPI_Recv(chunk, CHUNK_SIZE, MPI_INT, prev_rank, 0, cartesian_comm, MPI_STATUS_IGNORE);

            int offset = i * CHUNK_SIZE;
            if (!verify_data(chunk, offset, CHUNK_SIZE)) {
                printf("Proces %d: Fragment %d niepoprawny\n", cartesian_rank, i);
                EXIT_FAILURE;
            }

            if (next_rank != MPI_PROC_NULL) {
                MPI_Send(chunk, CHUNK_SIZE, MPI_INT, next_rank, 0, cartesian_comm);
            }
        }
    }

    if (cartesian_rank == 0) {
        free(vector);
    }

    return EXIT_SUCCESS;
}
