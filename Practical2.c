#include <omp.h>
#include <stdio.h>

int main() {

	int n,i=0;
	scanf("%d",&n);
	
	int a[n],c[n];
	
	for( i=0;i<n;i++) scanf("%d",&a[i]);
	
	int num;
	scanf("%d",&num);
		
	#pragma omp parallel for default(shared)
		for(i=0;i<n;i++)
			c[i] = a[i] + num;
	
	for( i=0;i<n;i++)
		printf("%d ",c[i]);
	return 0;
}


#include <omp.h>
#include <stdio.h>

int main() {

	int n,i;
	scanf("%d",&n);
		
	int a[n],c[n];
	
	for(i=0;i<n;i++) scanf("%d",&a[i]);
	
	int num;
	scanf("%d",&num);
	
	#pragma omp parallel for default(none) shared(a,c,n,num) private(i)
		for(i=0;i<n;i++)
			c[i] = a[i] + num;
	
	for(i=0;i<n;i++)
		printf("%d ",c[i]);
}



#include<stdio.h>
#include<omp.h>


int main(){
	
	int i,n;
	scanf("%d",&n);
	
	
	int a[n],c[n];
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	
	int num;
	scanf("%d",&num);
	
	printf("%d\n",omp_get_num_threads());
	#pragma omp parallel default(shared)
	{
 		int id, i, Nthrds, istart, iend;
 		
 		id = omp_get_thread_num();
 		Nthrds = omp_get_num_threads();
 		
 		istart = id*n/Nthrds;
	 	iend = (id+1)*n/Nthrds;
	 	
 		if(id == Nthrds-1) iend = n;
 		for(i = istart; i<iend; i++) 
		 	c[i] = num+a[i];
	}
	
	
 	for(i=0;i<n;i++)
 		printf("%d ",c[i]);
 	
}


#include <omp.h>
#include <stdio.h>

int main() {

	int n,i=0;
	scanf("%d",&n);
	
	int a[n],b[n],c[n];
	
	for( i=0;i<n;i++) scanf("%d",&a[i]);
	for( i=0;i<n;i++) scanf("%d",&b[i]);
	
	#pragma omp parallel for default(shared)
		for( i=0;i<n;i++)
			c[i] = a[i] + b[i];
	
	for( i=0;i<n;i++)
		printf("%d ",c[i]);
}

