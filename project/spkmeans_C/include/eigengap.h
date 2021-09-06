#ifndef PROJECT_Eigengap_Heuristic_H
#define PROJECT_Eigengap_Heuristic_H

#include "matrix_utils.h"
#include "jacobi.h"

/* Type 'T,k' holds a matrix of eigenvectors and an
 * array of corresponding eigenvalues.*/
typedef struct vector_values_pair {
	matrix* eigenvectors;
	double* eigenvalues;
	int n;
} vector_values_pair;

void stableSelectionSort(matrix* mat, double* a, int n);

matrix* eigengap_heuristic(vector_values_pair pair, int k);

int get_k(double* eigenvalues, int k, int n);

#endif
