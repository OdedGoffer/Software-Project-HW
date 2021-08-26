#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
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
		if (m == 0) {
			m = 1;
		}
		new_rows = (vector**) calloc((m * 2), sizeof(vector*));
		assert(new_rows != NULL);
		memcpy(new_rows, mat->rows, (mat->m) * sizeof(vector*));
		mat->rows = new_rows;
		mat->row_cap = 2 * m;
	}

	mat->rows[mat->m] = row;
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

/*mat diagonal must be positive, mat->m == mat->n, a = -0.5*/
void matrix_diagonal_pow(matrix* mat, double a) {
	double* val;
	int i;

	for (i=0; i<mat->m; i++) {
		val = mat->rows[i]->values[i];
		mat->rows[i]->values[i] = pow(val, a);
	}
}

matrix* matrix_eye(int n) {
	matrix* mat;
	int i;
	assert(!(n <= 0));

	mat = matrix_init(n, n);
	assert(mat != NULL);

	for (i = 0; i < m; i++) {
		mat->rows[i]->values[i] = 1;
	}

	return mat;
}

matrix* matrix_subtract(matrix* A, matrix* B) {
	matrix* ret;
	int i;
	int j;
	int n;

	n = A->n;
	ret = matrix_init(n, n);
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			ret->rows[i]->values[j] = A->rows[i]->values[j] - B->rows[i]->values[j];
		}
	}

	return ret;
}

matrix* matrix_mult(matrix* A, matrix* B) {
	matrix* ret;
	int i;
	int j;
	double sum = 0;
	int n;

	n = A->n;
	ret = matrix_init(n, n);
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			for (k=0; k<n; k++) {
				sum += A->rows[i]->values[k] * B->rows[k]->values[j];
			}
			ret->rows[i]->values[j] = sum;
			sum = 0;
		}
	}

	return ret;
}