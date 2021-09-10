#include "../include/WAM.h"
#include <assert.h>
#include <stdlib.h>
#include <math.h>

/*
 *
 * WAM
 *
 */

matrix* WAM(matrix* X) {
	matrix* ret;
	int n, i, j;
	double* vals;
	double dist;
	vector* new_vec;

	n = X->m;
	ret = matrix_init(n, 0);

	for (i = 0; i < n; i++) {
		vals = (double*) malloc(n * sizeof(double));
		assert(vals);
		for (j = 0; j < n; j++) {
			if (i == j) {
				vals[j] = 0;
			} else {
				dist = vector_dist(X->rows[i], X->rows[j]) / (-2);
				vals[j] = exp(dist);
			}
		}
		new_vec = vector_init(vals, n);
		matrix_add_row(ret, new_vec);
	}

	return ret;
}
