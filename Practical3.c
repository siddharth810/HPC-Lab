/* Program to compute Pi using Monte Carlo methods */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#define SEED 35791246
#define threads_cnt 10
int main(int argc, char* argv)
{
     int niter=0;
     double x,y;
     int i,count=0; /* # of points in the 1st quadrant of unit circle */
     double z;
     double pi;
     printf("Enter the number of iterations used to estimate pi: ");
     scanf("%d",&niter);

     /* initialize random numbers */
     srand(SEED);
     count=0;
     omp_set_num_threads(threads_cnt);
     #pragma omp parallel for default(shared)
     for ( i=0; i<niter; i++) 
     {
         x = (double)rand()/RAND_MAX;
         y = (double)rand()/RAND_MAX;
         // x = (double)(rand()%(niter+1))/niter;
         // y = (double)(rand()%(niter+1))/niter;
         z = x*x+y*y;
         printf("Thread %d works on iteration %d\n", omp_get_thread_num(), i);
         if (z<=1) count++;
     }

     pi=(double)count/niter*4;
     printf("# of trials= %d , estimate of pi is %g \n",niter,pi);
}

/*
    Problem Statement 2 : 
    Write a code in OpenMP for multiplication of 2D Matrix and a 1D vector 
    (Demonstrate the use of different clauses wherever applicable), 
    vary the size of your matrices from 200, 1000, 2000 and 3000 
    and measure the runtime with one thread 
    (Use functions in C in calculate the execution time or use GPROF) For each matrix size,
    change the number of threads from 2,4,8., 
    and plot the speedup versus the number of threads. 
    Explain whether or not the scaling behaviour is as expected.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//(1xn) * (nxm) = (1xm)  
//size of resultant matrix - 1xm

#define N 10 /* number of rows in matrix A */
#define M 500 /* number of columns in matrix B */


#define threads_cnt 32

int main (int argc, char *argv[])
{
    clock_t start, end;
  
    /* Recording the starting clock tick.*/
    start = clock();

    int tid, nthreads, i, j, k;

    //1d vector - a
    //2d matrix - b
    //resultant 1d vector - c
    int a[N], b[N][M], c[M];
    
    omp_set_num_threads(threads_cnt);

    printf("\nNumber of Threads : %d\n\n",threads_cnt);
    #pragma omp parallel shared(a,b,c,nthreads) private(tid,i,j,k)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
        }

        /*** Initialize matrices ***/
        #pragma omp for
            for (i=0; i<N; i++)
               a[i] = i;
        
        #pragma omp for
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    b[i][j]= i*j;
         
        #pragma omp for
            for (i=0; i<M; i++)
                c[i]= 0;

        #pragma omp for
            for (i=0; i<M; i++)
            {
                //printf("Thread=%d did row=%d\n",tid,i);
                for(j=0; j<N; j++)
                {
                    c[i] += a[j] * b[j][i];
                }
            }
     }

    printf("\n1D Vector :\n");
    for (i=0; i<N; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    
    printf("\n2D matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", b[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("Resultant vector:\n");
    for (i=0; i<M; i++)
    {
        printf("%d ", c[i]);
    }
    printf("\n");
    
    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("\nTime taken : %0.4f seconds\n",time_taken);
  
    return(0);
}

/* 
For 2D Matrix Addition of size 2000, Write a OpenMP code with the following: 
Use DYNAMIC schedule and set the loop iteration chunk size to various sizes 
when changing the size of your matrix. Analyze the speedup. 
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//size of resultant matrix - N*M

//size of matrix = 50*40 = 2000
#define N 50 /* number of rows */
#define M 40 /* number of columns */
#define threads_cnt 16

int main (int argc, char *argv[])
{
    clock_t start, end;
  
    /* Recording the starting clock tick.*/
    start = clock();

    int tid, nthreads, i, j, k;
    int a[N][M], b[N][M], c[N][M];
    /*** Create a parallel region explicitly scoping all variables ***/

    omp_set_num_threads(threads_cnt);

    int n_per_thread = N*M / threads_cnt;
    printf("\nNumber of Threads : %d",threads_cnt);
    printf("\nChunk Size : %d\n\n",n_per_thread);
    #pragma omp parallel default(shared) 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Starting matrix multiple example with %d threads\n",nthreads);
            printf("Initializing matrices...\n");
        }

        /*** Initialize matrices ***/
        #pragma omp for schedule(dynamic,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    a[i][j]= i+j;
        
        #pragma omp for schedule(dynamic,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    b[i][j]= i*j;
         
        #pragma omp for schedule(dynamic,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    c[i][j]= 0;

        
        printf("Thread %d starting matrix addition...\n",tid);
        #pragma omp for schedule(dynamic,n_per_thread )
            for (i=0; i<N; i++)
            {
                printf("Thread=%d did row=%d\n",tid,i);
                for(j=0; j<M; j++)
                	c[i][j] += a[i][j] + b[i][j];
            }
     } 
    
   

   	printf("First Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
    
    printf("Second Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", b[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("Resultant Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }
    
    printf("\n");

    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("\nTime taken : %0.4f\n",time_taken);
    return(0);
}

/* 
For 2D Matrix Addition of size 2000, Write a OpenMP code with the following: 
Demonstrate the use of nowait clause.
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//size of resultant matrix - N*M

#define N 5 /* number of rows */
#define M 5 /* number of columns */
#define threads_cnt 4

int main (int argc, char *argv[])
{
     clock_t start, end;
  
    /* Recording the starting clock tick.*/
    start = clock();

    int tid, nthreads, i, j, k;
    int a[N][M], b[N][M], c[N][M];
    /*** Create a parallel region explicitly scoping all variables ***/

    omp_set_num_threads(threads_cnt);

    int n_per_thread = N*M / threads_cnt;

    #pragma omp parallel default(shared) 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Starting matrix multiple example with %d threads\n",nthreads);
            printf("Initializing matrices...\n");
        }

        /*** Initialize matrices ***/
        #pragma omp for nowait
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    a[i][j]= i+j;
        
        #pragma omp for nowait
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    b[i][j]= i*j;
         
        #pragma omp for nowait
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    c[i][j]= 0;

        
        printf("Thread %d starting matrix addition...\n",tid);
        #pragma omp for nowait
            for (i=0; i<N; i++)
            {
                printf("Thread=%d did row=%d\n",tid,i);
                for(j=0; j<M; j++)
                	c[i][j] += a[i][j] + b[i][j];
            }
     } 
    
   

   	printf("First Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
    
    printf("Second Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", b[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("Resultant Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }
    
    printf("\n");

  
    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("\nTime taken : %0.4f\n",time_taken);
    return(0);
}


/* 
For 2D Matrix Addition of size 2000, Write a OpenMP code with the following: 
Use STATIC schedule and set the loop iteration chunk size to various sizes 
when changing the size of your matrix. Analyze the speedup. 
*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//size of resultant matrix - N*M

//size of matrix = 50*40 = 2000
#define N 50 /* number of rows */
#define M 40 /* number of columns */
#define threads_cnt 16

int main (int argc, char *argv[])
{
     clock_t start, end;
  
    /* Recording the starting clock tick.*/
    start = clock();

    int tid, nthreads, i, j, k;
    int a[N][M], b[N][M], c[N][M];
    /*** Create a parallel region explicitly scoping all variables ***/

    omp_set_num_threads(threads_cnt);

    int n_per_thread = N*M / threads_cnt;
    printf("\nNumber of Threads : %d",threads_cnt);
    printf("\nChunk Size : %d\n\n",n_per_thread);
    #pragma omp parallel default(shared) 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
        }

        /*** Initialize matrices ***/
        #pragma omp for schedule(static,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    a[i][j]= i+j;
        
        #pragma omp for schedule(static,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    b[i][j]= i*j;
         
        #pragma omp for schedule(static,n_per_thread )
            for (i=0; i<N; i++)
                for (j=0; j<M; j++)
                    c[i][j]= 0;

        
        printf("Thread %d starting matrix addition...\n",tid);
        #pragma omp for schedule(static,n_per_thread )
            for (i=0; i<N; i++)
            {
                printf("Thread=%d did row=%d\n",tid,i);
                for(j=0; j<M; j++)
                	c[i][j] += a[i][j] + b[i][j];
            }
     } 
    
   

   	printf("First Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", a[i][j]);
        printf("\n");
    }
    printf("\n");
    
    printf("Second Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", b[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("\nResultant Matrix:\n");
    for (i=0; i<N; i++)
    {
        for (j=0; j<M; j++)
            printf("%d ", c[i][j]);
        printf("\n");
    }
    
    printf("\n");

    end = clock();
    double time_taken = (double)(end - start) / (double)(CLOCKS_PER_SEC);
    printf("\nTime taken : %0.4f\n",time_taken);
    return(0);
}
