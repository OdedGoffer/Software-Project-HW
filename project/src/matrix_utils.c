#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/matrix_utils.h"
#include "../include/vector_utils.h"

matrix* matrix_init(int n, int m) {
	matrix* mat;
	vector** rows;
	int i;
	assert(!(n<0 || m<0));

	mat = (matrix*) malloc(sizeof(matrix));
	assert(mat != NULL);
	mat->n = n;
	mat->m = m;

	if (m == 0) {
		m = 1;
	}

	rows = (vector**) malloc(m * sizeof(vector*));
	assert(rows != NULL);
	mat->rows = rows;
	mat->row_cap = m;

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

/* Adds a new row to the end of the matrix.*/
void matrix_add_row(matrix* mat, vector* row) {
	vector** new_rows;
	int m;
	assert(mat != NULL && row != NULL);
	assert(mat->n == row->size);
	m = mat->m;

	if (mat->row_cap == m) {
		new_rows = (vector**) calloc((m * 2), sizeof(vector*));
		assert(new_rows != NULL);
		memcpy(new_rows, mat->rows, m * sizeof(vector*));
		mat->rows = new_rows;
		mat->row_cap = 2 * m;
	}

	mat->rows[m] = row;
	mat->m++;
}

matrix* matrix_copy(matrix* mat) {
	matrix* new_mat;
	int n;
	int m;
	int i;
	assert(mat != NULL);

	n = mat->n;
	m = mat->m;
	new_mat = matrix_init(n, m);

	for (i=0; i<m; i++) {
		new_mat->rows[i] = vector_copy(mat->rows[i]);
	}

	return new_mat;
}

void matrix_print(matrix* mat) {
	int i;
	assert(mat != NULL);

	printf("[\n");
	for (i=0; i<mat->m; i++) {
		printf("\t");
		vector_print(mat->rows[i]);
	}
	printf("]\n");
}

