#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char* argv[])
{
 
    
    #pragma omp parallel
    {
 
        printf("This is Siddharth Satpute - 2018BTECS00001 - %d\n",
               omp_get_thread_num());
    }
}
