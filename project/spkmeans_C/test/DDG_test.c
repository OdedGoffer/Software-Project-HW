#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/vector_utils.h"
#include "../include/matrix_utils.h"
#include "../include/DDG.h"

#define TEST "DDG"

void TEST_DDG_1() {

	matrix* mat;
	vector* v;
	vector* tmp;
	int n = 9;
	int m = 10;
	int i;

	mat = matrix_init(n, 0);
	v = vector_init_zero(n);
	for (i=0; i<n; i++) {
		v->values[i] = 1;
	}

	for (i=0; i<m; i++) {
		tmp = vector_copy(v);
		matrix_add_row(mat, v);
		v = tmp;
	}

	mat = DDG(mat);

	for (i=0; i<m; i++) {
		assertf(mat->rows[i]->values[i] != 3, "wrong value");
	}

	vector_free(tmp);
	matrix_free(mat);
}

int main() {
	start_test(TEST);

	TEST_DDG_1();

	end_test(TEST);
}
