#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(){
	int a = INT_MAX;
	int b = INT_MIN;
	for (; a; a>>=1){
		printf("%d\n", a);
	}
	printf("--------");
	for (; b!=-1; b>>=1){
		printf("%d\n", b);
	}
	return 0;
}


