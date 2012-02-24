#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#define pi2o6 1.6449340668482264364724151666460251892189499012067984

typedef double Real;

void iter(int length);

Real sum_array( double* vec , long limit );
Real * genarray ( long length );

int numprocs,rank,namelen;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;

int main(int argc, char ** argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);
	for(int i = 3; i < 15; i++){
		iter(i);
	}
	MPI_Finalize();
	return 0;
}

void iter(int length)
{

	int partition_size = ( 1 <<length  ) / numprocs,
		offset = ( 1<< length) %  partition_size;

	Real * array ;
	if ( rank == 0 ) {
		array = genarray(1<< length);
		Real * arr_os = array+offset, sum = 0 ;
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Send ( arr_os + i * partition_size, partition_size, MPI_DOUBLE, i , 100 , MPI_COMM_WORLD);
		}
	}
	
	if (rank == 0 ){
		Real sum = 0 , retsum = 0;
		for ( int i = 0 ;  i < partition_size+offset ; i++){
			sum +=array[i];	
		}
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Recv ( &retsum , 1 ,  MPI_DOUBLE , MPI_ANY_SOURCE , 101, MPI_COMM_WORLD, &status );
			sum+= retsum;
		}
		
		printf("s^%d\tSummen: %.16lf; feilen: %.16lf\n", length, sum, pi2o6 -sum);
	}
	else {
		Real sum = 0 ;
		array = malloc ( partition_size * sizeof( Real ) );
		MPI_Recv( array , partition_size , MPI_DOUBLE, 0 , 100 , MPI_COMM_WORLD , &status);
		/*
		 * vector received, do math
		 */

		for ( int i = 0 ; i < partition_size ; ++i )
		{
			sum += array[i];
		}

		/*
		 *I is sending the data back 
		 */
		MPI_Send(&sum ,1 ,MPI_DOUBLE , 0 , 101, MPI_COMM_WORLD );
	}

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

Real sum_array( double* vec , long limit )
{
	Real *end = vec+limit, sum = 0;
	for ( ; vec < end ; ++vec)
	{
		sum += *vec;
	}
	return sum;
}

// Fordi det gaar :)
inline void sum_array_2(Real * vec , long limit , double* acc){
	for (Real  * end = vec+limit; vec < end; *acc+=*(vec++)) ;
}
