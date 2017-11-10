//This program combines all the values of an array just like sampleMPI, but uses
//an extra command: Scatter. This command automatically takes an array and splits it
//throughout all the processes in the program.

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    //Initialize MPI COMM_WORLD. No need for arguments, so parameters are NULL
    MPI_Init(NULL, NULL);

    //Find how large the world is
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Find which process you are
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //Miscellaneous variales
    int const ARRAY_LENGTH = 15;
    //Assume the ARRAY_LENGTH is divisble by the number of processes
    int const PIECE_SIZE = ARRAY_LENGTH/world_size;
    int arrayPiece[PIECE_SIZE];
    int sum = 0;

    //All programs need to know what varaible "arr" is because all programs must
    //Come to the scatter command for command to activate. All other processes wait
    //until every process is accounted for. That's why MPI_Scatter has no if statement.
    //All processes come to it. When a slave process reaches it, it will ask what
    //&arr is. That first command only matters to the root (0) who has to supply
    //arr, but the slave processes will still complain. So arr is declared outside
    //of the if(world_rank==0) block.
    int arr[ARRAY_LENGTH] = {0};

    if(world_rank == 0) {
	//Fill the array wth value of which index it is (0,1,2,3,4,5,etc)
	for(int i=0; i<ARRAY_LENGTH; i++) {
	    arr[i] = i;
	}
    }

    //All processes come to this point and root sends arr in sections of PIECE_SIZE
    //to each process who places their piece in their individual "arraypiece"
    MPI_Scatter(&arr, PIECE_SIZE, MPI_INT, &arrayPiece, PIECE_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	//Add it up
	for(int i=0; i<PIECE_SIZE; i++) {
	    sum = sum + arrayPiece[i];
	}

    int fullsum = 0;
    //Receive all the messages and print out the answer
    MPI_Reduce(&sum, &fullsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    //Rank 0 prints the answer
    if(world_rank == 0) {
	printf("The sum of all the values in the array is %d\n", fullsum);
    }

    MPI_Finalize();

}
