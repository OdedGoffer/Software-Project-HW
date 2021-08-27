#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/jacobi_utils.h"
#include "../include/matrix_utils.h"

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
	assert(n > 1); /* If n==1 then Aij is not well-defined. */

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

	return (Aii - Ajj) / (2 * Aij);
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

	/* 		sign(theta)
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

matrix* get_P(matrix* A) {
	double theta, c, t, s;
	ituple ij;
	int n, i, j;
	matrix* P;
	assert(A != NULL);

	n = A->n;
	assert(n == A->m);
	P = matrix_eye(n);

	ij = get_largest_off_i_j(A);
	i = ij.i;
	j = ij.j;

	theta = get_theta(A, i, j);
	c = get_c(theta);
	t = get_t(c);
	s = get_s(c, t);

	matrix_set(i, i, P, c);
	matrix_set(j, j, P, c);
	matrix_set(i, j, P, s);
	matrix_set(j, i, P, -s);

	return P;
}
