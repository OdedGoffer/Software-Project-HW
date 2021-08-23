#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "../include/matrix_utils.h"
#include "../include/vector_utils.h"

matrix* WAM(matrix* X) {
	matrix* ret;
	int n;
	int size;
	double* vals;
	int i = 0;		
	int j = 0;
	double dist;
	vector* new_vec;

	n = X->m;
	size = X->n;
	ret = matrix_init(size, 0);

	for (i=0; i<n; i++) {
		vals = (double*)malloc(size * sizeof(double));
		assert(vals);
		for (j=0; j<n; j++) {
			if (i == j) {
				vals[j] = 0;
			} else { 
				dist = vector_dist(X->rows[i], X->rows[j]) / (-2);
				vals[j] = exp(dist);
			}
		}
		new_vec = vector_init(vals, size);
		matrix_add_row(ret, new_vec);
	}

	return ret;
}