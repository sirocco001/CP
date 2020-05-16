//José Manuel Turnes Pazos DNI:78809381-L
//Javier González Rodriguez DNI:32722360-F

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

#define DEBUG 1

/* Translation of the DNA bases
   A -> 0
   C -> 1
   G -> 2
   T -> 3
   N -> 4*/


#define M  1000 // Number of sequences
#define N  200000  // Number of bases per sequence

// The distance between two bases
int base_distance(int base1, int base2){

    if((base1 == 4) || (base2 == 4)){
        return 3;
    }

    if(base1 == base2) {
        return 0;
    }

    if((base1 == 0) && (base2 == 3)) {
        return 1;
    }

    if((base2 == 0) && (base1 == 3)) {
        return 1;
    }

    if((base1 == 1) && (base2 == 2)) {
        return 1;
    }

    if((base2 == 1) && (base1 == 2)) {
        return 1;
    }

return 2;
}

int main(int argc, char *argv[] ) 
{

    int i, j, numprocs, rank, limite;
    int nfilasbloque;
    int *data_aux1;
    int *data_aux2;
    int *res;
    int *data1, *data2;
    int *result;
    struct timeval  tv1, tv2;
    
  
	MPI_Init(&argc,&argv);
    MPI_Comm_size (MPI_COMM_WORLD , &numprocs );
    MPI_Comm_rank (MPI_COMM_WORLD , &rank );
	
	nfilasbloque=ceil(M/numprocs);
	
    data1 = (int *) malloc(M*N*sizeof(int));
    data2 = (int *) malloc(M*N*sizeof(int));
    result = (int *) malloc(M*sizeof(int));
	
    data_aux1 = (int *) malloc(nfilasbloque*N*sizeof(int));
	data_aux2 = (int *) malloc(nfilasbloque*N*sizeof(int));
	res = (int *) malloc(nfilasbloque*sizeof(int));
	
  /* Initialize Matrices */
    if(rank==0){
        for(i=0;i<M;i++) {
            for(j=0;j<N;j++) {
                data1[i*N+j] = (i+j)%5;
                data2[i*N+j] = ((i-j)*(i-j))%5;
            }
        }
	}
	
	if(MPI_Scatter(data1,nfilasbloque*M,MPI_INT,data_aux1,nfilasbloque*M,MPI_INT,0,MPI_COMM_WORLD)!=MPI_SUCCESS)
		fprintf(stderr,"Error proceso: %d\n", rank);
	if(MPI_Scatter(data2,nfilasbloque*M,MPI_INT,data_aux2,nfilasbloque*M,MPI_INT,0,MPI_COMM_WORLD)!=MPI_SUCCESS)
		fprintf(stderr,"Error proceso: %d\n", rank);
	
    gettimeofday(&tv1, NULL);

	if (rank == numprocs-1)
		limite = M - nfilasbloque * (numprocs -1);
	else
		limite = nfilasbloque;
	
    for(i=0;i<limite;i++) {
        res[i]=0;
        for(j=0;j<N;j++) {
            res[i] += base_distance(data1[i*N+j], data2[i*N+j]);
        }
    }

    gettimeofday(&tv2, NULL);
    
    if(MPI_Gather(res,nfilasbloque,MPI_INT,result,nfilasbloque,MPI_INT,0,MPI_COMM_WORLD)!=MPI_SUCCESS)
		fprintf(stderr,"Error proceso: %d\n", rank);
    
    int microseconds = (tv2.tv_usec - tv1.tv_usec)+ 1000000 * (tv2.tv_sec - tv1.tv_sec);

    /*Display result */
    if (DEBUG){
        for(i=0;i<M;i++) {
            printf(" %d \t ",result[i]);
        }
    } else {
        printf ("Time (seconds) = %lf\n", (double) microseconds/1E6);
    }    

    free(data1); free(data2); free(result);
	free(data_aux1); free(data_aux2); free(res);
return 0;
}

