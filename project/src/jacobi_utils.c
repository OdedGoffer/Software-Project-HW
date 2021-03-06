#include "../include/jacobi_utils.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

/*
 *
 * JACOBI UTILS
 *
 */

/* Returns index (i,j) of largest off-center element of 'mat'.*/
ituple get_largest_off_i_j(matrix* mat) {
	double val = 0.0;
	double curr;
	int n, i, j;
	ituple res = {
			0,
			1,
	};
	assert(mat != NULL);

	n = mat->n;
	assert(n == mat->m);
	assert(n > 1);

	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			curr = fabs(matrix_get(i, j, mat));
			if (curr > val) {
				val = curr;
				res.i = i;
				res.j = j;
			}
		}
	}
	return res;
}

double get_theta(matrix* mat, int i, int j) {
	double Aij, Aii, Ajj;
	assert(mat != NULL);

	Aij = matrix_get(i, j, mat);
	Aii = matrix_get(i, i, mat);
	Ajj = matrix_get(j, j, mat);

	return (Ajj - Aii) / (2 * Aij);
}

double sign(double x) {
	if (x < 0) {
		return -1.0;
	}
	return 1.0;
}

double get_t(double theta) {
	double theta_sqrd, abs_theta, sign_theta;

	theta_sqrd = pow(theta, 2);
	abs_theta = fabs(theta);
	sign_theta = sign(theta);

	return (sign_theta) / (abs_theta + sqrt(1 + theta_sqrd));

	/* 		    sign(theta)
	* 	----------------------------
	* 	abs(theta) + sqrt(1 + theta^2)
	*/
}

double get_c(double t) {
	double t_sqrd;

	t_sqrd = pow(t, 2);
	return 1 / (sqrt(1 + t_sqrd));
}

double get_s(double t, double c) {
	return c * t;
}

/* Calculate matrix 'P' and update A = A' as defined in jacobi algorithm.*/
matrix* get_P_and_update_A(matrix* A) {
	double theta, c, t, s, val;
	ituple ij;
	int n, i, j, p;
	matrix *P, *A_tmp;
	assert(A != NULL);

	n = A->n;
	assert(n == A->m);
	P = matrix_eye(n);

	/* Special case: n == 1, there is nothing to do, and so we return I. */
	if (n == 1) return P;

	ij = get_largest_off_i_j(A);
	i = ij.i;
	j = ij.j;

	/* Special case: Aij == 0, meaning A is diagonal, and so we return I. */
	if (!matrix_get(i, j, A)) return P;

	theta = get_theta(A, i, j);
	t = get_t(theta);
	c = get_c(t);
	s = get_s(c, t);

	matrix_set(i, i, P, c);
	matrix_set(j, j, P, c);
	matrix_set(i, j, P, s);
	matrix_set(j, i, P, -s);

	A_tmp = matrix_copy(A);

	for (p = 0; p < n; p++) {
		if (p == i || p == j) continue;

		val = (c * matrix_get(p, i, A_tmp)) - (s * matrix_get(p, j, A_tmp));
		matrix_set(p, i, A, val);
		matrix_set(i, p, A, val);

		val = (c * matrix_get(p, j, A_tmp)) + (s * matrix_get(p, i, A_tmp));
		matrix_set(p, j, A, val);
		matrix_set(j, p, A, val);
	}

	val = (pow(c, 2) * matrix_get(i, i, A_tmp)) + (pow(s, 2) * matrix_get(j, j, A_tmp)) - (2 * s * c * matrix_get(i, j, A_tmp));
	matrix_set(i, i, A, val);

	val = (pow(s, 2) * matrix_get(i, i, A_tmp)) + (pow(c, 2) * matrix_get(j, j, A_tmp)) + (2 * s * c * matrix_get(i, j, A_tmp));
	matrix_set(j, j, A, val);

	matrix_set(i, j, A, 0);
	matrix_set(j, i, A, 0);

	matrix_free(A_tmp);

	return P;
}
