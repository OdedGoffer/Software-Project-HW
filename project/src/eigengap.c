#include "../include/eigengap.h"
#include "../include/logger.h"

/*
 *
 * EIGENGAP HEURISTIC
 *
 */

/* Stable selection-sort on array 'a'. In addition, we reorder the rows of matrix
 * 'mat' using the same permutation as selection-sort on 'a'. */
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

/* Calculate K values from eigenvalues using eigengap-heuristic.
 * Retrun K if K != 0.*/
int get_k(double* eigenvalues, int k, int n) {
	double delta;
	double max_diff = 0;
	int i, index = 1;

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

/*Transpose and normalize rows of matrix u.*/
matrix* get_T(matrix* U) {
	matrix *T, *M;
	int i, j, n, m;
	double norm, val;
	vector* zero;

	M = matrix_transpose(U);
	matrix_free(U);

	n = M->n;
	m = M->m;
	T = matrix_init(n, m);
	zero = vector_init_zero(n);

	for (i=0; i<m; i++) {
		norm = vector_dist(M->rows[i], zero);
		if (norm == 0) error_occured();
		for (j=0; j<n; j++) {
			val = M->rows[i]->values[j] / norm;
			matrix_set(i, j, T, val);
		}
	}

	vector_free(zero);
	matrix_free(M);
	return T;
}

vectors_k_pair eigengap_heuristic(vectors_values_pair pair, int k) {
	vectors_k_pair verctors_pair;
	int n;

	n = pair.n;

	stableSelectionSort(pair.eigenvectors, pair.eigenvalues, n);

	k = get_k(pair.eigenvalues, k, n);
	matrix_slice(pair.eigenvectors, k);

	verctors_pair.vectors = get_T(pair.eigenvectors);
	verctors_pair.k = k;

	return verctors_pair;
}
