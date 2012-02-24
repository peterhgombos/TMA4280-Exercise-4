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

	int partition_size = ( 2 << 14 ) / numprocs;

	Real sum = 0 ;
	Real * array ;
	if ( rank == 0 ) {
		array = genarray( 2 << 14 );
		Real sum = 0 ;

		for ( int i = 0;  i < numprocs ; i++ ) {
			MPI_Send ( array + i * partition_size, partition_size, MPI_DOUBLE, i , 100 , MPI_COMM_WORLD);
		}
	}
	else {
		array = malloc ( partition_size * sizeof( Real ) );
		MPI_Recv( array , partition_size , MPI_DOUBLE, 0 , 100 , MPI_COMM_WORLD , &status);

		/*
		 * vector received, do math
		 */
	}
	{
		#pragma omp paralell for reduction( +:sum)
		for ( int i = 0 ; i < partition_size ; ++i )
		{
			sum += array[i];
		}
		printf("%lf\n", sum);

		/*
		 *I is sending the data back 
		 */
		MPI_Send(&sum ,1 ,MPI_DOUBLE , 0 , 101, MPI_COMM_WORLD );
	}

	if (rank == 0 ){
		double summ = sum;
#pragma omp paralell for reduction( + : sum )
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Recv ( &sum , 1 ,  MPI_DOUBLE , MPI_ANY_SOURCE , 101, MPI_COMM_WORLD, &status );
		}

		sum+=summ;
		printf("Summen: %.16lf; feilen: %.16lf\n", sum, pi2o6 -sum);
	}


	MPI_Finalize();
	return 0;
}

/*Real master(double *array  , long length )
  {
  array = genarray( length );
  return array;
  }

  int slave(  )
  {
  }*/

Real * genarray ( long length )
{
	Real *array = malloc ( length*sizeof(double) );
	for (int i = 1 ;i < length  ; ++i)
	{
		array[i] = pow ( i , -2 );
	}
	return array;
}

// Fordi det går :)
inline void sum_array_2(Real * vec , long limit , double* acc){
	for (Real  * end = vec+limit; vec < end; *acc+=*(vec++)) ;
}
