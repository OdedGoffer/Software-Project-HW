#include "../include/jacobi.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/jacobi_utils.h"

/*
 *
 * JACOBI
 *
 */

#define JACOBI_MAX_ITER 100
#define EPSILON 0.001

int check_convergence(matrix* A, matrix* A_tag) {
	double A_off, A_tag_off;
	assert((A) && (A_tag));

	A_off = matrix_off(A);
	A_tag_off = matrix_off(A_tag);
	if(fabs(A_off - A_tag_off) < EPSILON) return 1;
	return 0;
}

/* Returns (P_mult * P) */
matrix* multiply_P(matrix* P, matrix* P_mult) {
	matrix *P_mult_old;
	assert(P);

	if (P_mult != NULL) {
		P_mult_old = P_mult;
		P_mult = matrix_mult(P_mult_old, P);
		matrix_free(P_mult_old);
		return P_mult;
	}

	return matrix_copy(P);
}

/* Returns diagonal elements of 'A' as an array.*/
double* diagonal_to_array(matrix* A) {
	double* res;
	int n, i;
	assert(A);

	n = A->n;
	assert(n == A->m);

	res = (double*)malloc(n*sizeof(double));
	assert(res);
	for (i = 0; i < n; i++) {
		res[i] = A->rows[i]->values[i];
	}
	return res;
}

vectors_values_pair jacobi(matrix* A) {
	int i;
	int done = 0;
	matrix* A_tag, *P, *P_mult;
	vectors_values_pair res;

	assert(A);
	P_mult = NULL;

	for (i = 0; i < JACOBI_MAX_ITER; i++) {
		A_tag = matrix_copy(A);
		P = get_P_and_update_A(A);
		P_mult = multiply_P(P, P_mult);

		if (check_convergence(A, A_tag)) done = 1;

		matrix_free(P);
		matrix_free(A_tag);

		if (done == 1) break;
	}

	res.eigenvectors = matrix_transpose(P_mult);
	res.eigenvalues = diagonal_to_array(A);
	res.n = A->n;
	matrix_free(P_mult);
	matrix_free(A);

	return res;
}

void eigenvectors_free(vectors_values_pair vvp) {
	matrix_free(vvp.eigenvectors);
	free(vvp.eigenvalues);
}
