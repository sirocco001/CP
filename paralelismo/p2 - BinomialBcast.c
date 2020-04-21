//José Manuel Turnes Pazos DNI:78809381-L
//Javier González Rodriguez DNI:32722360-F

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int MPI_BinomialBcast(void *buff,int count,MPI_Datatype datatype,int root,MPI_Comm comm)
{
	int i,aux,numprocs,rank;
	MPI_Status status;
	MPI_Comm_size (comm , &numprocs );
    MPI_Comm_rank (comm , &rank );
	printf("%d",numprocs);
	for(i=0;i<ceil(sqrt(numprocs));i++){
		aux=pow(2,i-1);
		if(rank<aux){
			if(rank+aux<numprocs){
				MPI_Send(buff,count,datatype,rank+aux,0,comm);
			}
		}else if(rank>=aux && rank<aux+aux-1){
			MPI_Recv(buff,count,datatype,rank-aux,0,comm,&status);
		}
		MPI_Barrier(comm);
	}
}

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
	}
		// for (i=1;i<numprocs;i++){
			// MPI_Send(&n,1,MPI_INT,i,0,MPI_COMM_WORLD);
		// }
    	// }else
		// MPI_Recv(&n,1,MPI_INT,0,0,MPI_COMM_WORLD,&status);
		MPI_BinomialBcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

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
