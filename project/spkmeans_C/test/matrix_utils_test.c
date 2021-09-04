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
	for (i = 0; i < m; i++) {
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
	for (i = 0; i < n; i++) {
		v->values[i] = i;
	}

	for (i = 0; i < x; i++) {
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
	int n = 10;
	int i;

	mat = matrix_init(n, 0);

	for (i = 0; i < n; i++) {
		v = vector_init_zero(n);
		v->values[i] = 4;
		matrix_add_row(mat, v);
	}

	matrix_diagonal_pow(mat, -0.5);

	for (i = 0; i < n; i++) {
		assertf(mat->rows[i]->values[i] == 0.5, "wrong number on diagonal");
	}

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
	for (i = 0; i < n; i++) {
		v->values[i] = i;
	}

	for (i = 0; i < n; i++) {
		tmp = vector_copy(v);
		matrix_add_row(mat1, v);
		v = tmp;
		tmp = vector_copy(v);
		matrix_add_row(mat2, v);
		v = tmp;
	}

	mat1 = matrix_subtract(mat1, mat2);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
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
	for (i = 0; i < n; i++) {
		v1->values[i] = i;
	}

	for (i = 0; i < n; i++) {
		tmp = vector_copy(v1);
		matrix_add_row(mat1, v1);
		v1 = tmp;
		tmp = vector_copy(v2);
		matrix_add_row(mat2, v2);
		v2 = tmp;
	}

	mat1 = matrix_mult(mat1, mat2);

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			assertf(mat1->rows[i]->values[j] == 0, "problema amigo");
		}
	}

	vector_free(tmp);
	matrix_free(mat1);
	matrix_free(mat2);
}

void TEST_matirx_off() {
	matrix* mat;
	double off;

	mat = matrix_init(2, 2);
	matrix_set(0, 0, mat, 4);
	matrix_set(0, 1, mat, 2);
	matrix_set(1, 0, mat, 1);
	matrix_set(1, 1, mat, 0);
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 ]
	 */

	off = matrix_off(mat);
	assertf(off == 5.0, "Wrong value for matrix_off");

	matrix_free(mat);

}

void TEST_matrix_transpose() {
	matrix *mat, *mat_transpose;

	mat = matrix_init(2, 2);
	matrix_set(0, 0, mat, 4);
	matrix_set(0, 1, mat, 2);
	matrix_set(1, 0, mat, 1);
	matrix_set(1, 1, mat, 0);
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 ]
	 */

	mat_transpose = matrix_transpose(mat);

	assertf(mat_transpose->rows[0]->values[1] == 1, "wrong transpose value");
	assertf(mat_transpose->rows[1]->values[0] == 2, "wrong transpose value");

	matrix_free(mat);
}

void TEST_matrix_swap() {
	matrix *mat;

	mat = matrix_init(2, 2);
	matrix_set(0, 0, mat, 4);
	matrix_set(0, 1, mat, 2);
	matrix_set(1, 0, mat, 1);
	matrix_set(1, 1, mat, 0);
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 ]
	 */

	matrix_swap(mat, 0, 1);

	assertf(mat->rows[0]->values[0] == 1, "wrong 0,0 value");
	assertf(mat->rows[0]->values[1] == 0, "wrong 0,1 value");
	assertf(mat->rows[1]->values[0] == 4, "wrong 1,0 value");
	assertf(mat->rows[1]->values[1] == 2, "wrong 1,1 value");

	matrix_free(mat);
}

void TEST_matrix_slice() {
	matrix *mat;

	mat = matrix_init(2, 3);
	matrix_set(0, 0, mat, 4);
	matrix_set(0, 1, mat, 2);
	matrix_set(1, 0, mat, 1);
	matrix_set(1, 1, mat, 0);
	matrix_set(2, 0, mat, 5);
	matrix_set(2, 1, mat, 3);
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 
	 	 5, 3 ]
	 */

	matrix_slice(mat, 1);

	assertf(mat->rows[0]->values[0] == 4, "wrong 0,0 value");
	assertf(mat->rows[0]->values[1] == 2, "wrong 0,1 value");
	assertf(mat->row_cap == 1, "wrong raw_cap value");

	matrix_free(mat);
}

int main() {
	start_test(TEST);

	TEST_matrix_init();
	TEST_matrix_add_row();
	TEST_matrix_diagonal_pow();
	TEST_matrix_subtract();
	TEST_matrix_mult();
	TEST_matirx_off();
	TEST_matrix_transpose();
	TEST_matrix_swap();
	TEST_matrix_slice();

	end_test(TEST);
}
