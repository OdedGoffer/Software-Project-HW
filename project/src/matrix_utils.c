#include <assert.h>
#include <stdlib.h>
#include "../include/matrix_utils.h"
#include "../include/vector_utils.h"

matrix* matrix_init(int n, int m) {
	matrix* mat;
	vector** rows;
	int i;

	mat = (matrix*) malloc(sizeof(matrix));
	assert(mat != NULL);
	mat->n = n;
	mat->m = m;

	rows = (vector**) malloc(m * sizeof(vector*));
	assert(rows != NULL);
	mat->rows = rows;

	for (i = 0; i < m; i++) {
		rows[i] = vector_init_zero(n);
	}

	return mat;
}

void matrix_free(matrix* mat) {
	int i;

	for (i=0; i<mat->m; i++) {
		free(mat->rows[i]);
	}
	free(mat->rows);
	free(mat);
}

