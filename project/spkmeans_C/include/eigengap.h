#ifndef PROJECT_Eigengap_Heuristic_H
#define PROJECT_Eigengap_Heuristic_H

#include "matrix_utils.h"
#include "jacobi.h"

/* Type 'verctors_k_pair' holds a matrix of normalized vectors and the 
 * updates value of k (after eigengap algorithm if required).*/
typedef struct verctors_k_pair {
	matrix* vectors;
	int k;
} verctors_k_pair;

void stableSelectionSort(matrix* mat, double* a, int n);

verctors_k_pair eigengap_heuristic(vector_values_pair pair, int k);

int get_k(double* eigenvalues, int k, int n);

#endif
