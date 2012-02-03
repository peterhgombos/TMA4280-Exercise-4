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

double sum_of_inf(){
	return pow(M_PI, 2)/6;
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
	double k;
	double s = sum_of_inf();
	printf("s = %.10f\n", s);
	for (int i = 3; i <= 14; i++){
		k = pow(2, i);
		printf("s - 2^%d \t %.10f\n", i, s - sum_vector( gen_vector(k), k));
	}
	return 0;
}
