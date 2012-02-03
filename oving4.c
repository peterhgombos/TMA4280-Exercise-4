#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oving4.h"

double * gen_vector ( long n ) {
	double * a = malloc ( sizeof(double) * n ) ,
		   *b;
	b = a;
	for ( int i = 1 ; i < n + 1 ; i++ , b++ ) {
		*b = pow ( i , -2 );
	}
	return a;
}

double sum_vector ( double * vec , long n ){
	double * limit = vec + n , out = 0;

	double * ptr = vec, * ptr2 = vec; 
	if ( n == 1 ) {
		return *vec;
	}
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

int main (int argc , char ** argv){
	double * vec = gen_vector(atoi(argv[1]));
	double s = sum_vector( vec, atoi(argv[1]));
	printf("%.10f\n", s);
	return 0;
}
