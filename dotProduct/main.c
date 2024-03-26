// Template C to x86 call
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define n 134217728 //2^27 max

extern void asmhello();

int main(int argc, char* argv[]) {
    srand(time(NULL));

    float vect_A[n] = { 0 };
    float vect_B[n] = { 0 };

    //initialize vector array
    for (int i = 0; i < n; i++) {
        vect_A[i] = ((float)rand() / (float)(RAND_MAX)) * 10.0;
        //printf("vect_A[%d] = %f\n", i, vect_A[i]);
        vect_B[i] = ((float)rand() / (float)(RAND_MAX)) * 10.0;
        //printf("vect_B[%d] = %f\n", i, vect_B[i]);
    }

    //C function call
    float result = getDotProduct(vect_A, vect_B);
    printf("Dot product is %f \n", result);

    //x86_64 function call
	asmhello();
	
	return 0;
}
