#include "../include/matrix_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
 *
 * MATRIX UTILS
 *
 */

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

	if (mat == NULL) return;

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

/* Returns a deep copy of matrix 'mat'.*/
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

	for (i=0; i<mat->m; i++) {
		vector_print(mat->rows[i]);
	}
}

/*'mat' must be a square matrix with positive diagonal values.*/
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

/*Returns a pointer to the Identity matrix of size n*/
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

/*Returns A - B. Returns a new matrix.*/
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

/*Returns A * B. Returns a new matrix.*/
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

/* Calculates off(mat)^2 as defined in Jacobi algorithm.*/
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
			sum += pow(mat->rows[j]->values[i], 2);
		}
	}
	return sum;
}

void matrix_set(int i, int j, matrix* mat, double val) {
	assert(i >= 0 && i < mat->m);
	assert(j >= 0 && j < mat->n);
	mat->rows[i]->values[j] = val;
}

double matrix_get(int i, int j, matrix* mat) {
	assert(i >= 0 && i < mat->m);
	assert(j >= 0 && i < mat->n);
	return mat->rows[i]->values[j];
}

matrix* matrix_transpose(matrix* mat) {
	matrix* mat_transpose;
	int n_old, m_old, n_new, m_new, i, j;
	double val;
	assert(mat);

	n_old = mat->n;
	m_old = mat->m;

	n_new = m_old;
	m_new = n_old;

	mat_transpose = matrix_init(n_new, m_new);

	for (i = 0; i < n_old; i++) {
		for (j = 0; j < m_old; j++) {
			val = mat->rows[j]->values[i];
			mat_transpose->rows[i]->values[j] = val;
		}
	}
	return mat_transpose;
}

/* Swap rows 'i' and 'j' of matrix 'mat'.*/
void matrix_swap(matrix* mat, int i, int j) {
	vector* tmp;

	tmp = mat->rows[i];
	mat->rows[i] = mat->rows[j];
	mat->rows[j] = tmp; 
}

/* Returns a slice of matrix 'mat' containing only first 'k' rows.*/
void matrix_slice(matrix* mat, int k) {
	int i, m;
	vector** new_rows;
	vector** old_rows;

	old_rows = mat->rows;
	m = mat->m; 
	for (i=k; i<m; i++) {
		vector_free(mat->rows[i]);
	}

	new_rows = (vector**) malloc(sizeof(vector*) * k);
	assert(new_rows);
	memcpy(new_rows, old_rows, sizeof(vector*) * k);
	mat->rows = new_rows;
	mat->m = k;
	mat->row_cap = k; 
	free(old_rows);
}
