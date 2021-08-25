#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/matrix_utils.h"
#include "../include/vector_utils.h"

matrix* DDG(matrix* W) {
	matrix* ret;
	int n;
	double* vals;
	double sum;
	int i = 0;		
	vector* vec;

	n = W->n;
	ret = matrix_init(n, 0);

	for (i=0; i<n; i++) {
		vals = (double*)calloc(n * sizeof(double));
		assert(vals);
		sum = vector_values_sum(W->rows[i]);
		vals[i] = sum;
		vec = vector_init(vals, n);
		matrix_add_row(ret, vec);
	}

	matrix_diagonal_pow(ret, -0.5);

	return ret;
}