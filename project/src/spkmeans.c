#include <stdlib.h>
#include <stdio.h>
#include "../include/vector_utils.h"

int main() {
    double *vals;
    int N = 4;
    int i;
    vector *v;

    vals = (double *)malloc(sizeof(double)*N);
    for (i=0;i<N;i++){
        vals[i] = (double)i;
    }
    v = vector_init(vals, N);
    printf("DONE!\n");
    return 0;
}