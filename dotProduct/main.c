// Template C to x86 call
#include "dotProduct.h"
#include "setup.h"

void view_array(float array[], long size);
void simple_init(float vect_A[], float vect_B[], long size);
void random_vectors(float vect_A[], float vect_B[], long size);
extern float asmDotProduct(float vect_A[], float vect_B[], long size);

double average_double(double array[], int size);
void print_summary(double time_elapsed_c[], double time_elapsed_asm[], int size);
void free_vectors(double vect_A[], double vect_B[]);

// int main(int argc, char* argv[])
int main()
{
    srand(time(NULL));

    // float vect_A[n] = { 0 };
    // float vect_B[n] = { 0 };
    float* vect_A = (float*)calloc(n, sizeof(float));
    float* vect_B = (float*)calloc(n, sizeof(float));

    clock_t start_time_c;
    clock_t end_time_c;
    double time_elapsed_c[RUNS] = { 0 };

    clock_t start_time_asm;
    clock_t end_time_asm;
    double time_elapsed_asm[RUNS] = { 0 };

    // initialize vector array
    random_vectors(vect_A, vect_B, n);
    // simple_init(vect_A, vect_B, n);

    // simply show the vectors
    // printf("----vect_a---\n");
    // view_array(vect_A, n);
    // printf("\n");
    // printf("----vect_b---\n");
    // view_array(vect_B, n);
    float result_c = 0.0;
    float result_asm = 0.0;

    for (int i = 0; i < RUNS; i++) {
        start_time_c = clock();
        result_c = getDotProduct(vect_A, vect_B); // C function call
        end_time_c = clock();
        time_elapsed_c[i] = (double)(end_time_c - start_time_c) / CLOCKS_PER_SEC;
        printf("[Elapsed time: %lf]\n", time_elapsed_c[i]);
        printf("RUN %d: [C] Dot product is %f \n", i, result_c);

        start_time_asm = clock();
        result_asm = asmDotProduct(vect_A, vect_B, n); // x86_64 function call
        end_time_asm = clock();
        time_elapsed_asm[i] = (double)(end_time_asm - start_time_asm) / CLOCKS_PER_SEC;
        printf("[Elapsed time: %lf]\n", time_elapsed_asm[i]);
        printf("RUN %d: [ASM] Dot product is %f \n", i, result_asm);

        if (result_c == result_asm) {
            printf("BOTH ARE EQUAL\n");
        } else {
            printf("BOTH ARE NOT EQUAL\n");
        }
        printf("----------\n");
    }

    print_summary(time_elapsed_c, time_elapsed_asm, RUNS);

    double average_et_c = average_double(time_elapsed_c, RUNS);
    double average_et_asm = average_double(time_elapsed_asm, RUNS);

    printf("\n");
    printf("Average time elapsed of C implementation:\n\t%lfms\n",
        average_et_c);
    printf("Average time elapsed of x64 implementation:\n\t%lfms\n",
        average_et_asm);

    // free_vectors(vect_A, vect_B);
    free(vect_A);
    free(vect_B);
    return 0;
}

void view_array(float array[], long size)
{
    for (int i = 0; i < size; i++) {
        printf("float[%d] = %f\n", i, array[i]);
    }
}

void simple_init(float vect_A[], float vect_B[], long size)
{
    for (long int i = 0; i < size; i++) {
        vect_A[i] = (float)i;
        vect_B[i] = (float)i;
    }
}

void random_vectors(float vect_A[], float vect_B[], long int size)
{
    for (long int i = 0; i < size; i++) {
        vect_A[i] = ((float)rand() / (float)(RAND_MAX)) * 10.0;
        // printf("vect_A[%d] = %f\n", i, vect_A[i]);
        vect_B[i] = ((float)rand() / (float)(RAND_MAX)) * 10.0;
        // printf("vect_B[%d] = %f\n", i, vect_B[i]);
    }
}

double average_double(double array[], int size)
{
    double average = 0.0;
    for (int i = 0; i < size; i++) {
        average += array[i];
    }

    average /= size;
    return average;
}

void print_summary(double time_elapsed_c[], double time_elapsed_asm[], int size)
{
    printf("------------------------------\n");
    printf("|%28s|\n", "Elapsed time in all runs");
    printf("------------------------------\n");
    printf("|%5s|%11s|%10s|\n", "Run #", "C", "x64");
    printf("------------------------------\n");
    for (int i = 0; i < size; i++) {
        printf("|%-5d|%11lf|%10lf|\n", i, time_elapsed_c[i], time_elapsed_c[i]);
    }
    printf("------------------------------\n\n");
}

void free_vectors(double vect_A[], double vect_B[])
{
    free(vect_A);
    free(vect_B);
}
