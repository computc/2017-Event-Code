//This program adds all the values of an array (created by first process)

//The first process (rank 0, a.k.a the "root") splits the array to all other
//processes. Each process adds up their sliver of the array and then sends back
//the partial sum. The final array adds up these slivers and gets the answer.

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    //Initialize MPI space and COMM_WORLD.
    //Because we won't need argc and argv (arguments passed into program from command line),
    //the MPI_Init command has only NULL
    MPI_Init(NULL, NULL);

        //Parallel Section: Every process runs through this code:

    //Get how many processes are running
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    //Get which process you are
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    //Initialize miscellaneous variables
    int const ARRAY_LENGTH = 25;
      //Size of array each process gets. Assume ARRAY_LENGTH is divisible by the number of processes
    int const PIECE_SIZE = ARRAY_LENGTH/world_size;
    //Initialize small array to hold 
    int arrayPiece[PIECE_SIZE];
    int sum = 0;
    //When using MPI_Recv, a status is returned which helps for error troubleshooting
    MPI_Status status;

    if(world_rank == 0) {	//Root process
	//Make an array
	int arr[ARRAY_LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24};
	//Splits it and sends to other processes.
	    for(int rankToSend=1; rankToSend<world_size; rankToSend++) {
		for(int arrIndex=0; arrIndex<PIECE_SIZE; arrIndex++) {
		    arrayPiece[arrIndex]=arr[(rankToSend*PIECE_SIZE) + arrIndex];
		}
		//Send piece of array (filled arraypiece) to process
		MPI_Send(arrayPiece, PIECE_SIZE, MPI_INT, rankToSend, 0, MPI_COMM_WORLD);
	    }
	//Initialize the section of the array rank 0 will be adding up
	    for(int j=0; j<PIECE_SIZE; j++) {
		arrayPiece[j]=arr[j];
	    }
    }
    else {		//All other processes (slave processes)
	//Receive the segment of an array. &arrayPiece is where received data goes. Second argument denotes how large the array is (1 if simple variable)
	MPI_Recv(&arrayPiece, PIECE_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

	//NOTE: THE & MODIFIER BEFORE A PARAMETER IN C++ MEANS IT IS A "WRITABLE VARIABLE"
	//	THIS MEANS METHOD IS MEANT TO MODIFY THAT VARIABLE
	//	IT IS, IN ESSENCE, A "MUTABLE VARIABLE" IN A METHOD
	//	THIS IS HOW EACH PROCESS "RECEIVES" A VARIABLE INTO &arrayPiece

    }

	//Add it up
	for(int i=0; i<PIECE_SIZE; i++) {
	    sum = sum + arrayPiece[i];
	}

    int fullsum = 0;
    //MPI_Reduce works when ALL PROCESSES in the COMM_WORLD use it. All values
    //are put together into the root process. The MPI_SUM is used to denote the
    //operation used to combine "sum values" together into the root.

    //Receive all the messages and print out the answer
    MPI_Reduce(&sum, &fullsum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    //Root now has complete sum. Now PRINT IT!!
    if(world_rank == 0) {
	printf("The sum of all the values in the array is %d\n", fullsum);
    }

    MPI_Finalize();

}
