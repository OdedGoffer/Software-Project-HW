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

	rows = NULL;
	mat = (matrix*) malloc(sizeof(matrix));
	assert(mat != NULL);
	if (m != 0) {
		rows = (vector**) malloc(m * sizeof(vector*));
		assert(rows != NULL);
	}
	mat->n = n;
	mat->m = m;
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
	double val;
	int i;
	assert(mat->m == mat->n);

	for (i=0; i<mat->m; i++) {
		val = mat->rows[i]->values[i];
		assert(val > 0);
		val = pow(val, fabs(a));
		if (a < 0) {
			val = 1.0/val;
		}
		mat->rows[i]->values[i] = val;
	}
}

matrix* matrix_eye(int n) {
	matrix* mat;
	int i;
	assert(!(n <= 0));

	mat = matrix_init(n, n);
	assert(mat != NULL);

	for (i = 0; i < n; i++) {
		mat->rows[i]->values[i] = 1;
	}

	return mat;
}

matrix* matrix_subtract(matrix* A, matrix* B) {
	matrix* ret;
	int i, j, n;

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
	int i, j, k, n;
	double sum = 0;
	matrix* ret;

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
double matrix_off(matrix* mat) {
	int n, m, i, j;
	double sum = 0;
	assert(mat != NULL);

	n = mat->n;
	m = mat->m;
	assert(m == n);

	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			if (i == j) {
				continue;
			}
			sum += pow(mat->rows[m]->values[n], 2);
		}
	}
	return sum;
}
