#include "test_utils/test_utils.h"
#include "../include/jacobi_utils.h"

#define TEST "jacobi_utils"

void TEST_get_largest_off_i_j() {
	matrix* mat;
	ituple res;

	mat = matrix_init(2, 2);
	mat->rows[0]->values[0] = 4;
	mat->rows[0]->values[1] = 2;
	mat->rows[1]->values[0] = 1;
	mat->rows[1]->values[1] = 0;
	/* mat =
	 * [ 4, 2
	 * 	 1, 0 ]
	 */

	res = get_largest_off_i_j(mat);
	assertf(res.i == 0, "wrong i value");
	assertf(res.j == 1, "wrong j value");

	matrix_free(mat);
}

void TEST_get_P() {
	matrix* mat;
	matrix* P;
	double expected = 1.0 / sqrt(2);

	mat = matrix_eye(2);
	matrix_set(0, 1, mat, 1);
	matrix_set(1, 0, mat, 1);

	P = get_P(mat);

	assertf(matrix_get(0, 0, P) == expected, "Wrong value at A(0,0)");
	assertf(matrix_get(0, 1, P) == expected, "Wrong value at A(0,1)");
	assertf(matrix_get(1, 0, P) == -expected, "Wrong value at A(1,0)");
	assertf(matrix_get(0, 1, P) == expected, "Wrong value at A(0,1)");

	matrix_free(P);
	matrix_free(mat);
}

int main() {
	start_test(TEST);

	TEST_get_largest_off_i_j();
	TEST_get_P();

	end_test(TEST);
}
