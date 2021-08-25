#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/matrix_utils.h"
#include "../include/vector_utils.h"

matrix* LNORM(matrix* W, matrix* D) {
	matrix* ret;
	matrix* eye;
	int n;

	n = W->n;
	eye = matrix_eye(n);

	ret = matrix_mult(D, W);
	ret = matrix_mult(ret, D);
	ret = matrix_subtract(eye, ret);

	return ret;
}