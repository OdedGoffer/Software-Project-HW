#ifndef PROJECT_Eigengap_Heuristic_H
#define PROJECT_Eigengap_Heuristic_H

#include "matrix_utils.h"
#include "jacobi.h"

/*
 *
 * EIGENGAP
 *
 */

/* Type 'vectors_k_pair' holds a matrix of normalized vectors and the
 * updates value of k (using Eigengap algorithm if required).*/
typedef struct vectors_k_pair {
	matrix* vectors;
	int k;
} vectors_k_pair;

void stableSelectionSort(matrix* mat, double* a, int n);

vectors_k_pair eigengap_heuristic(vectors_values_pair pair, int k);

int get_k(double* eigenvalues, int k, int n);

#endif
