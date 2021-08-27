#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "../include/vector_utils.h"
#include "../include/matrix_utils.h"
#include "../include/WAM.h"
#include "../include/DDG.h"
#include "../include/LNORM.h"


#define TEST "LNORM"

void TEST_LNORM_1() {

	matrix *X, *W, *D, *L;
	vector* v;
	vector* tmp;
	int n = 5;
	int i, j;

	X = matrix_init(n, 0);
	v = vector_init_zero(n);

	for (i=0; i<n; i++) {
		tmp = vector_copy(v);
		matrix_add_row(X, v);
		v = tmp;
	}

	W = WAM(X);
	D = DDG(W);
	L = LNORM(W, D);

	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			if (i == j) {
				assertf(L->rows[i]->values[j] == 1, "wrong value on diagonal");
			} else {
				assertf(L->rows[i]->values[j] == -0.25, "wrong value outside diagonal");
			}
		}
	}

	vector_free(tmp);
	matrix_free(X);
	matrix_free(W);
	matrix_free(D);
	matrix_free(L);
}

int main() {
	start_test(TEST);

	TEST_LNORM_1();

	end_test(TEST);
}
