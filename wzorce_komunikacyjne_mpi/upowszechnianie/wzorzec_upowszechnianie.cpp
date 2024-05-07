#include <mpi.h>
#include <iostream>
#include "nwd.h"
#include <cmath>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int number;
    switch (world_rank) {
        case 0: number = 326851121; break;
        case 1: number = 718064159; break;
        case 2: number = 9737333; break;
        case 3: number = 997525853; break;
        case 4: number = 37139213; break;
        default: number = 174440041; break;
    }

    int local_nwd = number;
    int jump = 1;

    MPI_Barrier(MPI_COMM_WORLD); 

    while (jump < world_size) {
        int recv_rank = (world_rank - jump + world_size) % world_size;
        int send_rank = (world_rank + jump) % world_size;

        int received_value = local_nwd;

        MPI_Status status;
        MPI_Sendrecv(&local_nwd, 1, MPI_INT, send_rank, 0, 
                     &received_value, 1, MPI_INT, recv_rank, 0, 
                     MPI_COMM_WORLD, &status);

        local_nwd = nwd(local_nwd, received_value);

        jump *= 2;
    }

    printf("%d %d\n", world_rank, local_nwd);

    MPI_Finalize();
    return 0;
}
