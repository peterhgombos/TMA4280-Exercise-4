#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#define pi2o6 1.6449340668482264364724151666460251892189499012067984


double sum_array( double* vec , long limit );
double * genarray ( long length );

int main(int argc, char ** argv)
{
	//free (arr);
	int numprocs,rank,namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	MPI_Status status;
	MPI_Init( &argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

	int partition_size = ( 2 << 14 ) / numprocs,
		offset = ( 2 << 14 ) %  partition_size;

	double * array ;
	if ( rank == 0 ) {
		array = genarray( 2 << 14 );
		double * arr_os = array+offset, sum = 0 ;
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Send ( arr_os + i * partition_size, partition_size, MPI_DOUBLE, i , 100 , MPI_COMM_WORLD);
		}
	}
	else 
	{
		double sum = 0 ;
		array = malloc ( partition_size * sizeof( double ) );
		MPI_Recv( array , partition_size , MPI_DOUBLE, 0 , 100 , MPI_COMM_WORLD , &status);
		/*
		 * vector received, do math
		 */

		for ( int i = 0 ; i < partition_size ; ++i )
		{
			sum += array[i];
		}
		//printf("%lf\n", sum );

		/*
		 *I is sending the data back 
		 */
		//printf("returning: Process %d on %s out of %d ::::sum is: %lf\n", rank, processor_name, numprocs, sum);
		MPI_Send(&sum ,1 ,MPI_DOUBLE , 0 , 101, MPI_COMM_WORLD );
	}

	if (rank == 0 ){
		double sum = 0 , retsum = 0;
		for ( int i = 0 ;  i < partition_size+offset ; i++){
			sum +=array[i];	
		}
		for ( int i = 1;  i < numprocs ; i++ ) {
			MPI_Recv ( &retsum , 1 ,  MPI_DOUBLE , MPI_ANY_SOURCE , 101, MPI_COMM_WORLD, &status );
			sum+= retsum;
			//printf("gathering :Process %d on %s out of %d\n", rank, processor_name, numprocs);
		}
		
		printf("Summen: %.16lf; feilen: %.16lf\n", sum, pi2o6 -sum);
	}
	MPI_Finalize();
	return 0;
}

/*double master(double *array  , long length )
  {
  array = genarray( length );
  return array;
  }

  int slave(  )
  {
  }*/

double * genarray ( long length )
{
	double *array = malloc ( length*sizeof(double) );
	for (int i = 1 ;i < length  ; ++i)
	{
		array[i] = pow ( i , -2 );
	}
	return array;
}

double sum_array( double* vec , long limit )
{
	double *end = vec+limit, sum = 0;
	//for ( ; vec < end ; sum = *(vec++) );
	for ( ; vec < end ; ++vec)
	{
		sum += *vec;
	}
	return sum;
}

// Fordi det går :)
inline void sum_array_2(double * vec , long limit , double* acc){
	for (double  * end = vec+limit; vec < end; *acc+=*(vec++)) ;
}
