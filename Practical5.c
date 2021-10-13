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

int main() {
	
	int first = 1,second = 2 ,third = 3,fourth = 4;
	
	MPI_Init(NULL,NULL);
	
	int rank,size;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	if(rank == 0) 
		MPI_Send(&first,1,MPI_INT,1,0,MPI_COMM_WORLD);
	
	if(rank == 1) {

		MPI_Recv(&first,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		printf("process rank-  %d , data received %d \n",rank,first);
		
		MPI_Send(&second,1,MPI_INT,2,12,MPI_COMM_WORLD);
	}
	
	if(rank == 2) {
		MPI_Recv(&second,1,MPI_INT,1,12,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		printf("process rank-  %d , data received %d \n",rank,second);
		MPI_Send(&third,1,MPI_INT,3,13,MPI_COMM_WORLD);
	}
	
	if(rank == 3) {
		MPI_Recv(&third,1,MPI_INT,2,13,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		printf("process rank-  %d , data received %d \n",rank,third);
		
		MPI_Send(&fourth,1,MPI_INT,0,14,MPI_COMM_WORLD);
	}
	
	if(rank == 0) {
		MPI_Recv(&fourth,1,MPI_INT,3,14,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		
		printf("process rank-  %d , data received %d \n",rank,fourth);
	}
	
	MPI_Finalize();
	
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
