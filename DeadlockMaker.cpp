#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    int num = 3;
    if(world_rank == 0) {
        num = 5;
        MPI_Recv(&num, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if(world_rank == 1) {
        MPI_Recv(&num, 1, MPI_INT, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if(world_rank == 2) {
        MPI_Recv(&num, 1, MPI_INT, 3, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else if(world_rank == 3) {
        MPI_Recv(&num, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Print off a message
    printf("Processor %s, rank %d"
           " out of %d processors say that num=%d\n",
           processor_name, world_rank, world_size, num);

    // Finalize the MPI environment.
    MPI_Finalize();
}


