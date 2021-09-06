#ifndef PROJECT_JACOBI_H
#define PROJECT_JACOBI_H

#include "matrix_utils.h"

/* Type 'vector_values_pair' holds a matrix of eigenvectors and an
 * array of corresponding eigenvalues.*/
typedef struct vector_values_pair {
	matrix* eigenvectors;
	double* eigenvalues;
	int n;
} vector_values_pair;

/* Use Jacobi method to calculate eigenvectors.*/
vector_values_pair jacobi(matrix* A);

void eigenvectors_free(vector_values_pair vvp);

#endif
