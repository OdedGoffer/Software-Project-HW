#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/vector_utils.h"
#include "../include/matrix_utils.h"
#include "../include/WAM.h"

#define TEST "WAM"

void TEST_WAM_1() {

	matrix* mat;
	vector* v;
	vector* tmp;
	int n = 7;
	int m = 9;
	int i, j;

	mat = matrix_init(n, 0);
	v = vector_init_zero(n);
	for (i=0; i<n; i++) {
		v->values[i] = i;
	}

	for (i=0; i<m; i++) {
		tmp = vector_copy(v);
		matrix_add_row(mat, v);
		v = tmp;
		assertf(mat->rows[i] != NULL, "new row is NULL");
	}

	mat = WAM(mat);
	for (i=0; i<m; i++) {
		for (j=0; j<n; j++) {
			if (i == j) {
				assertf(mat->rows[i]->values[j] == 0, "wrong value on diagonal")
			} else {
				assertf(mat->rows[i]->values[j] == 1, "wrong value")
			}
		}
	}

	vector_free(tmp);
	matrix_free(mat);

}

int main() {
	start_test(TEST);

	TEST_WAM_1();

	end_test(TEST);
}
