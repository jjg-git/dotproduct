#include "dotProduct.h"
#include "setup.h"

float getDotProduct(float vect_A[], float vect_B[])
{
    float product = 0.0;
    for (int i = 0; i < n; i++) {
        product = product + (vect_A[i] * vect_B[i]);
    }
    return product;
}
