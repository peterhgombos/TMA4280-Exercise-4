#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oving4.h"
#define pi2o6 1.6449340668482264364724151666460251892189499012067984

double * gen_vector ( long n ) {
	double * a = malloc ( sizeof(double) * n );
	double * b;
	b = a;
	for ( int i = 1 ; i < n + 1 ; i++ , b++ ) {
		*b = pow ( i , -2 );
	}
	return a;
}


double sum_vector ( double * vec , long n )
{
	double * limit = vec + n , out = 0;

	double * ptr = vec, * ptr2 = vec; 
	//Some sanity checks 
	if ( n == 1 ) {
		return vec[0];
	}
	else if ( n==0){
		return 0;
	}
	
	// Sum the contents of the vector and store the sum in the beginning of the array. The loop will stop at the second last element should it be odd membered, the if afterwards will handle that case;
	for ( ; ptr+1 < limit; ptr+=2, ptr2++ ){
		*ptr2= ptr[0] + ptr[1];
	}
	if ( ptr < limit ) {
		ptr2[0]= ptr[0];
	}

	if (n==2) {
		return ptr2[-1];
	}
	else {
		return sum_vector( vec, ( n / 2 ) + ( n & 1 ) );
	}
	return  out;
}

double sum_vector_openMP(double * vec, long n )
{
	double * limit = vec + n , sum = 0;

	double * ptr = vec, * ptr2 = vec; 
	//Some sanity checks 
	if ( n == 1 ) {
		return vec[0];
	}
	else if ( n==0){
		return 0.0;
	}
	
	// Sum the contents of the vector and store the sum in the beginning of the array. The loop will stop at the second last element should it be odd membered, the if afterwards will handle that case;
#pragma omp paralell for reduction( + : sum )
	for ( ; ptr < limit; ++ptr ){
		sum= ptr[0];
	}
	return sum;
}

inline void sum_vector_2(Real * vec , long limit , double* acc){
	for (Real  * end = vec+limit; vec < end; *acc+=*(vec++)) ;
}

int main (int argc , char ** argv){
	double k;
	printf("s = %.10f\n", pi2o6);
	for (int i = 3; i <= 14; i++){
		k = pow(2, i);
		printf("s - 2^%d \t %.10f\n", i, pi2o6 - sum_vector( gen_vector(k), k));
	}
	return 0;
}
