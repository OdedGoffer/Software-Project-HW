#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/vector_utils.h"
#include "../include/matrix_utils.h"

#define TEST "matrix_utils"

void TEST_matrix_init() {
	int m = 7;
	int n = 14;
	int i;
	matrix* mat;
	vector* v;

	mat = matrix_init(n, m);

	assertf(mat != NULL, "matrix is NULL");
	assertf(mat->m == m && mat->n == n, "wrong matrix dimension");
	for (i=0; i<m; i++) {
		v = mat->rows[i];
		assertf(v != NULL, "rows not initialized");
	}

	matrix_free(mat);
}

void TEST_matrix_add_row() {
	matrix* mat;
	vector* v;
	vector* tmp;
	int n = 7;
	int x = 9;
	int i;

	mat = matrix_init(n, 0);
	v = vector_init_zero(n);
	for (i=0; i<n; i++) {
		v->values[i] = i;
	}

	for (i=0; i<x; i++) {
		tmp = vector_copy(v);
		matrix_add_row(mat, v);
		v = tmp;
		assertf(mat->rows[i] != NULL, "new row is NULL");
	}

	assertf(mat->m == x, "wrong number of rows");

	vector_free(tmp);
	matrix_free(mat);

}

void TEST_matrix_diagonal_pow() {
	matrix* mat;
	vector* v;
	vector* tmp;
	int n = 10;
	int i;

	mat = matrix_init(n, 0);

	for (i=0; i<n; i++) {
		v->values[i] = 4;
		tmp = vector_copy(v);
		matrix_add_row(mat, v);
		v = tmp;
	}

	matrix_diagonal_pow(mat, -2);

	for (i=0; i<n; i++) {
		assertf(mat->row[i]->values[i] == 0.5, "wrong number on diagonal");
	}

	vector_free(tmp);
	matrix_free(mat);

}

void TEST_matrix_subtract() {
	matrix* mat1;
	matrix* mat2;
	vector* v;
	vector* tmp;
	int n = 10;
	int i;
	int j;

	mat1 = matrix_init(n, 0);
	mat2 = matrix_init(n, 0);
	v = vector_init_zero(n);
	for (i=0; i<n; i++) {
		v->values[i] = i;
	}

	for (i=0; i<n; i++) {
		tmp = vector_copy(v);
		matrix_add_row(mat1, v);
		v = tmp;
		tmp = vector_copy(v);
		matrix_add_row(mat2, v);
		v = tmp;
	}

	mat1 = matrix_subtract(mat1, mat2);

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			assertf(mat1->rows[i]->values[j] == 0, "problema amigo");
		}
	}

	vector_free(tmp);
	matrix_free(mat1);
	matrix_free(mat2);
}

void TEST_matrix_mult() {
	matrix* mat1;
	matrix* mat2;
	vector* v1;
	vector* v2;
	vector* tmp;
	int n = 10;
	int i;
	int j;

	mat1 = matrix_init(n, 0);
	mat2 = matrix_init(n, 0);
	v1 = vector_init_zero(n);
	v2 = vector_init_zero(n);
	for (i=0; i<n; i++) {
		v1->values[i] = i;
	}

	for (i=0; i<n; i++) {
		tmp = vector_copy(v1);
		matrix_add_row(mat1, v1);
		v1 = tmp;
		tmp = vector_copy(v2);
		matrix_add_row(mat2, v2);
		v2 = tmp;
	}

	mat1 = matrix_mult(mat1, mat2);

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			assertf(mat1->rows[i]->values[j] == 0, "problema amigo");
		}
	}

	vector_free(tmp);
	matrix_free(mat1);
	matrix_free(mat2);
}

int main() {
	start_test(TEST);

	TEST_matrix_init();
	TEST_matrix_add_row();
	TEST_matrix_diagonal_pow();
	TEST_matrix_subtract();
	TEST_matrix_mult();

	end_test(TEST);
}