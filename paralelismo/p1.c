//José Manuel Turnes Pazos DNI:78809381-L
//Javier González Rodriguez DNI:

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, j, prime, done = 0, n, count, numprocs, rank, parcial;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size (MPI_COMM_WORLD , &numprocs );
    MPI_Comm_rank (MPI_COMM_WORLD , &rank );
    
    while (!done)
    {
	if (rank==0){
        	printf("Enter the maximum number to check for primes: (0 quits) \n");
        	scanf("%d",&n);
		for (i=1;i<numprocs;i++){
			MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
		}
    	}else
		MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
	

        if (n == 0) break;

        count = 0;
	
        for (i = 2+rank; i < n; i+=numprocs) {
            prime = 1;

            // Check if any number lower than i is multiple
            for (j = 2; j < i; j++) {
                if((i%j) == 0) {
                   prime = 0;
                   break;
                }
            }
            count += prime;
        }
	

	if (rank!=0){
		MPI_Send(&count,1,MPI_INT,0,1,MPI_COMM_WORLD);
		
	}else{
		for (i=0;i<numprocs-1;i++){
			MPI_Recv(&parcial,1,MPI_INT,MPI_ANY_SOURCE,1,MPI_COMM_WORLD,&status);
			count = count + parcial;
		}
		printf("The number of primes lower than %d is %d\n", n, count);
	}
    }
		MPI_Finalize();
}
