//Ques1
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])  
{
      int numtasks, rank, next, prev, buf[2], tag1=1, tag2=2;
      MPI_Request reqs[4];
      MPI_Status stats[4];

      MPI_Init(&argc,&argv);
      MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      prev = rank-1;
      next = rank+1;
      if (rank == 0)  
            prev = numtasks - 1;
      if (rank == (numtasks - 1))  
            next = 0;

      MPI_Irecv(&buf[0], 1, MPI_INT, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
      printf("Process %d received with tag %d from process %d\n", prev, tag1,rank);
      MPI_Irecv(&buf[1], 1, MPI_INT, next, tag2, MPI_COMM_WORLD, &reqs[1]);
      printf("Process %d received with tag %d from process %d\n", next, tag2,rank);

      MPI_Isend(&rank, 1, MPI_INT, prev, tag2, MPI_COMM_WORLD, &reqs[2]);
      MPI_Isend(&rank, 1, MPI_INT, next, tag1, MPI_COMM_WORLD, &reqs[3]);
        
   

      MPI_Waitall(4, reqs, stats);

      MPI_Finalize();
}

//Ques2

#include<mpi.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	int rank, size;
    int tag, destination, count;
    int buffer; 

    tag = 1234;
    destination = 1; 
    count = 1; 
    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    if (rank == 0) {
        buffer = 9999;
        MPI_Isend(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD, &request); 
    }

    if (rank == destination) {
        MPI_Irecv(&buffer, count, MPI_INT, 0, tag, MPI_COMM_WORLD, &request); 
    }
    MPI_Wait(&request, &status); 

    if (rank == 0) {
        printf("processor %d sent %d\n", rank, buffer);
    }
    if (rank == destination) {
        printf("processor %d rcv %d\n", rank, buffer);
    }

    MPI_Finalize();

	return 0;
}



//Ques3

#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h>
 
int main(int argc, char* argv[])
{ 
 int np, pid; 
 MPI_Init(&argc, &argv); 
 MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
 MPI_Comm_size(MPI_COMM_WORLD, &np); 
 int partial_prod = 1; 
 if (pid == 0) 
 { 
	int a[] = { 1, 2, 3, 4, 5}; 
	for(int i = 0; i < 5; i++) 
		partial_prod *= a[i]; 
 }
 else if (pid == 1)
 { 
	int a[] = {6, 7, 8, 9, 10}; 
	for(int i = 0; i < 5; i++) 
	partial_prod *= a[i]; 
 }  
 int sum; 
 MPI_Reduce(&partial_prod, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); 
 if (pid == 0)
 {
 	printf("Sum of array is : %d\n", sum); 
 } 
 MPI_Finalize(); 
 return 0; 
}
