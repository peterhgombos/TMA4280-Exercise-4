#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#define pi2o6 1.6449340668482264364724151666460251892189499012067984

typedef double Real;


Real sum_array( double* vec , long limit );
Real * genarray ( long length );

int main(int argc, char ** argv)
{
	int numprocs,rank,namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status status;
	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

	int partition_size = ( 1 << 14 ) / numprocs,
		offset = ( 1 << 14 ) %  partition_size;

	Real * array ;
	if ( rank == 0 ) {
		array = genarray( 1 << 14 );
		Real * arr_os = array+offset, sum = 0 ;
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Send ( arr_os + i * partition_size, partition_size, MPI_DOUBLE, i , 100 , MPI_COMM_WORLD);
		}
	}
	
	if (rank == 0 ){
		Real sum = 0 , retsum = 0;
#pragma omp paralell for reduction( +: sum) schedule(static)
		for ( int i = 0 ;  i < partition_size+offset ; i++){
			sum +=array[i];	
		}
#pragma omp paralell for reduction( +: sum) 
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Recv ( &retsum , 1 ,  MPI_DOUBLE , MPI_ANY_SOURCE , 101, MPI_COMM_WORLD, &status );
			sum+= retsum;
		}
		
		printf("Summen: %.16lf; feilen: %.16lf\n", sum, pi2o6 -sum);
	}
	else {
		Real sum = 0 ;
		array = malloc ( partition_size * sizeof( Real ) );
		MPI_Recv( array , partition_size , MPI_DOUBLE, 0 , 100 , MPI_COMM_WORLD , &status);
		/*
		 * vector received, do math
		 */

#pragma omp parlell for reduction( +: sum) schedule(static)
		for ( int i = 0 ; i < partition_size ; ++i )
		{
			sum += array[i];
		}

		/*
		 *I is sending the data back 
		 */
		MPI_Send(&sum ,1 ,MPI_DOUBLE , 0 , 101, MPI_COMM_WORLD );
	}

	MPI_Finalize();
	return 0;
}


Real * genarray ( long length )
{
	Real *array = malloc ( length*sizeof(double) );
	for (int i = 1 ;i < length  ; ++i)
	{
		array[i] = pow ( i , -2 );
	}
	return array;
}

