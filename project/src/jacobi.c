#include "../include/jacobi.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/jacobi_utils.h"

#define JACOBI_MAX_ITER 100
#define EPSILON 0.01

int check_convergence(matrix* A, matrix* A_tag) {
	double A_off, A_tag_off;
	assert((A) && (A_tag));

	A_off = matrix_off(A);
	A_tag_off = matrix_off(A_tag);
	if(fabs(A_off - A_tag_off) < EPSILON) return 1;
	return 0;
}

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

double* diagonal_to_array(matrix* A) {
	double* res;
	int n, m, i;
	assert(A);

	n = A->n;
	m = A->m;
	assert(n == m);

	res = (double*)malloc(n*sizeof(double));
	assert(res);
	for (i = 0; i < n; i++) {
		res[i] = A->rows[i]->values[i];
	}
	return res;
}

vector_values_pair jacobi(matrix* A) {
	int i;
	int done = 0;
	matrix* A_tag, *P, *P_mult;
	vector_values_pair res;

	assert(A);
	P_mult = NULL;

	for (i = 0; i < JACOBI_MAX_ITER; i++) {
		P = get_P(A);
		A_tag = get_A_tag(A, P);
		P_mult = multiply_P(P, P_mult);

		if (check_convergence(A, A_tag)) {
			done = 1;
		}

		matrix_free(P);
		matrix_free(A);
		A = A_tag;

		if (done == 1) break;
	}

	if (!done) {
		matrix_free(P_mult);
		matrix_free(A);
		res.eigenvectors = NULL;
		res.eigenvalues = NULL;
		return res;
	}

	res.eigenvectors = P_mult;
	res.eigenvalues = diagonal_to_array(A);
	res.n = A->n;
	matrix_free(A);

	return res;
}

void eigenvectors_free(vector_values_pair vvp) {
	matrix_free(vvp.eigenvectors);
	free(vvp.eigenvalues);
}
