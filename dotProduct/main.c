// Template C to x86 call
#include "dotProduct.h"
#include "setup.h"

#define VECTOR_FILE 0

void view_array(float array[], long size);
void view_vectors(float vect_A[], float vect_B[], long size);
void simple_init(float vect_A[], float vect_B[], long size);
void random_vectors(float vect_A[], float vect_B[], long size);
extern float asmDotProduct(float vect_A[], float vect_B[], long size);

double average_double(double array[], int size);
void print_summary(double time_elapsed_c[], double time_elapsed_asm[], int size);
void free_vectors(float vect_A[], float vect_B[]);
void print_file(double time_elapsed_c[], double time_elapsed_asm[], int size);

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
    // random_vectors(vect_A, vect_B, n);
    // simple_init(vect_A, vect_B, n);

    // simply show the vectors
    // printf("----vect_a---\n");
    // view_array(vect_A, n);
    // printf("\n");
    // printf("----vect_b---\n");
    // view_array(vect_B, n);
    float result_c = 0.0;
    float result_asm = 0.0;

#if VECTOR_FILE == 1
    FILE* vector_file = fopen("vector.txt", "w");
    short has_vector_file = vector_file != NULL;

    if (!has_vector_file) {
        printf("ERROR! Can't write a text file to list all vectors.\n\n");
    }
#endif
    random_vectors(vect_A, vect_B, n);

    for (int i = 0; i < RUNS; i++) {
        printf("RUN %d...\n", i + 1);

        // simply show the vectors
        // view_vectors(vect_A, vect_B, n);
#if VECTOR_FILE == 1
        if (has_vector_file) {
            fprintf(vector_file, "RUN %d... ", i + 1);
            fprintf(vector_file, "vector A:\n");
            for (int j = 0; j < n; j++) {
                fprintf(vector_file, "%f\n", vect_A[j]);
            }
            fprintf(vector_file, "\n");

            fprintf(vector_file, "vector B:\n");
            for (int j = 0; j < n; j++) {
                fprintf(vector_file, "%f\n", vect_B[j]);
            }
            fprintf(vector_file, "-------------\n");
        }
#endif

        // Benchmarking the C kernel

        start_time_c = clock();
        result_c = getDotProduct(vect_A, vect_B); // C function call
        end_time_c = clock();
        time_elapsed_c[i] = (double)(end_time_c - start_time_c) / CLOCKS_PER_SEC;

        printf("[Elapsed time: %lf]\n", time_elapsed_c[i]);
        printf("[C] Dot product is %f \n", result_c);

        printf("\n");
        // Benchmarking the x86_64 kernel

        start_time_asm = clock();
        result_asm = asmDotProduct(vect_A, vect_B, n); // x86_64 function call
        end_time_asm = clock();
        time_elapsed_asm[i] = (double)(end_time_asm - start_time_asm) / CLOCKS_PER_SEC;

        printf("[Elapsed time: %lf]\n", time_elapsed_asm[i]);
        printf("[ASM] Dot product is %f \n", result_asm);

        if (result_c == result_asm) {
            printf("Both outputs are equal.\n");
        } else {
            printf("Both outputs are not equal.\n");
        }
        printf("-----------------------------\n");
    }
#if VECTOR_FILE == 1
    if (!has_vector_file) {
        fclose(vector_file);
    }
#endif

    print_summary(time_elapsed_c, time_elapsed_asm, RUNS);

    double average_et_c = average_double(time_elapsed_c, RUNS);
    double average_et_asm = average_double(time_elapsed_asm, RUNS);

    printf("\n");
    printf("Average time elapsed of C implementation:\n\t%lfms\n",
        average_et_c);
    printf("Average time elapsed of x64 implementation:\n\t%lfms\n",
        average_et_asm);

    // Output file
    print_file(time_elapsed_c, time_elapsed_asm, RUNS);

    // free_vectors(vect_A, vect_B);
    free_vectors(vect_A, vect_B);
    return 0;
}

void view_array(float array[], long size)
{
    for (int i = 0; i < size; i++) {
        // printf("float[%d] = %f\n", i, array[i]);
        printf("%f\n", array[i]);
    }
}

void view_vectors(float vect_A[], float vect_B[], long size)
{
    printf("vect_a:\n");
    view_array(vect_A, n);
    printf("\n");
    printf("vect_b:\n");
    view_array(vect_B, n);
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
        printf("|%-5d|%11lf|%10lf|\n", i, time_elapsed_c[i], time_elapsed_asm[i]);
    }
    printf("------------------------------\n\n");
}

void print_file(double time_elapsed_c[], double time_elapsed_asm[], int size)
{
    FILE* file = fopen("output.csv", "w");
    fprintf(file, "%s,%s\n", "C", "x64");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%lf,%lf\n", i, time_elapsed_c[i], time_elapsed_asm[i]);
    }
    fclose(file);
}

void free_vectors(float vect_A[], float vect_B[])
{
    free(vect_A);
    free(vect_B);
}
