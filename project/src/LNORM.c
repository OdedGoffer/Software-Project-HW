#include "../include/LNORM.h"

/*
 *
 * LNORM
 *
 */

matrix* LNORM(matrix* W, matrix* D) {
	matrix *ret1, *ret2, *ret3, *eye;
	int n;

	n = W->n;
	eye = matrix_eye(n);
	matrix_diagonal_pow(D, -0.5);

	ret1 = matrix_mult(D, W);
	ret2 = matrix_mult(ret1, D);
	ret3 = matrix_subtract(eye, ret2);

	matrix_free(eye);
	matrix_free(ret1);
	matrix_free(ret2);
	return ret3;
}
