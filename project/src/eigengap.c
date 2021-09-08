#include "../include/eigengap.h"
#include <stdlib.h>

void stableSelectionSort(matrix* mat, double* a, int n) {
	int i, j, min; 
	double key;
	vector* vec;

	for (i = 0; i < n - 1; i++) {
        min = i;

        for (j = i + 1; j < n; j++) {

        	if (a[min] > a[j]) {
        		min = j;
        	}
 
            key = a[min];
            vec = mat->rows[min];

            while (min > i) {
            	a[min] = a[min - 1];
            	matrix_swap(mat, min, min-1);
                min--;
            }
            a[i] = key;
            mat->rows[i] = vec;
        }
    }
}

int get_k(double* eigenvalues, int k, int n) {
	double delta;
	double max_diff = 0;
	int i, index;

	if (k != 0) {
		return k;
	}

	for (i=0; i<n-1; i++) {
		delta = eigenvalues[i+1] - eigenvalues[i];
		if (delta > max_diff) {
			max_diff = delta;
			index = i+1;
		}
	}

	return index;
}

matrix* get_T(matrix* U) {
	matrix* T;
	int i, j, n, m;
	double norm, val;
	vector* zero;

	n = U->n;
	m = U->m;
	T = matrix_init(n, m);
	zero = vector_init_zero(n);

	for (i=0; i<m; i++) {
		norm = vector_dist(U->rows[i], zero);
		for (j=0; j<n; j++) {
			val = U->rows[i]->values[j] / norm;
			matrix_set(i, j, T, val);
		}
	}

	vector_free(zero);
	return T;
}

vectors_k_pair eigengap_heuristic(vectors_values_pair pair, int k) {
	vectors_k_pair verctors_pair;
	int n;
	matrix *mat, *U;
	double* eigenvalues;

	n = pair.n;
	mat = matrix_transpose(pair.eigenvectors);
	eigenvalues = pair.eigenvalues;

	stableSelectionSort(mat, eigenvalues, n);

	k = get_k(eigenvalues, k, n);
	matrix_slice(mat, k);
	U = matrix_transpose(mat);

	verctors_pair.vectors = get_T(U);
	verctors_pair.k = k;

	matrix_free(pair.eigenvectors);
	matrix_free(mat);
	matrix_free(U);
	free(eigenvalues);
	return verctors_pair;
}
