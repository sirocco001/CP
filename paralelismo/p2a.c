//José Manuel Turnes Pazos DNI:78809381-L
//Javier González Rodriguez DNI:32722360-F

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int i, j, prime, done = 0, n, count, numprocs, rank, total;
    //MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size (MPI_COMM_WORLD , &numprocs );
    MPI_Comm_rank (MPI_COMM_WORLD , &rank );
    
    while (!done)
    {
		if (rank==0){
        	printf("Enter the maximum number to check for primes: (0 quits) \n");
        	scanf("%d",&n);
		}
		MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
        
        if (n == 0) break;

        count = 0;
        total = 0;
	
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
		MPI_Reduce(&count,&total,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
		if (rank==0){
			printf("The number of primes lower than %d is %d\n", n, total);
		}
    }

    	MPI_Barrier(MPI_COMM_WORLD);
		MPI_Finalize();
}
