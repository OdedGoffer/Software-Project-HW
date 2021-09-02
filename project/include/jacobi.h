#ifndef PROJECT_JACOBI_H
#define PROJECT_JACOBI_H

#include "matrix_utils.h"

typedef struct vector_values_pair {
	matrix * eigenvectors;
	double* eigenvalues;
	int n;
} vector_values_pair;

vector_values_pair jacobi(matrix* A);

void eigenvectors_free(vector_values_pair vvp);

#endif
