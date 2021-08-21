#include "test_utils/test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

int main() {
	start_test(TEST);

	TEST_matrix_init();

	end_test(TEST);
}
