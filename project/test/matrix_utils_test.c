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

void TEST_matirx_off(){
	matrix* mat;
	double off;

	mat = matrix_init(2, 2);
	mat->rows[0]->values[0] = 4;
	mat->rows[0]->values[1] = 2;
	mat->rows[1]->values[0] = 1;
	mat->rows[1]->values[1] = 0;
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 ]
	 */

	off = matrix_off(mat);
	assertf(off == 3.0, "Wrong value for matrix_off");

	matrix_free(mat);

}

int main() {
	start_test(TEST);

	TEST_matrix_init();
	TEST_matrix_add_row();

	end_test(TEST);
}
