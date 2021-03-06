#include "../include/DDG.h"
#include <assert.h>
#include <stdlib.h>

/*
 *
 * DDG
 *
 */

matrix* DDG(matrix* W) {
	matrix* ret;
	int m, i;
	double* vals;
	double sum;
	vector* vec;

	m = W->m;
	ret = matrix_init(m, 0);

	for (i=0; i<m; i++) {
		vals = (double*)calloc(m, sizeof(double));
		assert(vals);
		sum = vector_values_sum(W->rows[i]);
		vals[i] = sum;
		vec = vector_init(vals, m);
		matrix_add_row(ret, vec);
	}

	return ret;
}
