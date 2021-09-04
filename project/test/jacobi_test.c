#include "test_utils/test_utils.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "../include/jacobi.h"

#define TEST "jacobi"
#define EPSILON 0.0001

matrix* create_matrix() {
	matrix* mat;

	mat = matrix_init(3, 3);
	matrix_set(0, 0, mat, 3);
	matrix_set(0, 1, mat, 2);
	matrix_set(0, 2, mat, 4);
	matrix_set(1, 0, mat, 2);
	matrix_set(1, 1, mat, 0);
	matrix_set(1, 2, mat, 2);
	matrix_set(2, 0, mat, 4);
	matrix_set(2, 1, mat, 2);
	matrix_set(2, 2, mat, 3);

	/* mat =
	 * [ 3, 2, 4
	 * 	 2, 0, 2
	 * 	 4, 2, 3 ]
	 */

	return mat;
}

void TEST_jacobi() {
	matrix* A;
	vector_values_pair res;

	A = create_matrix();

	res = jacobi(A);

	assertf(fabs(res.eigenvalues[0] - (-1.0)) < EPSILON, "wrong eigenvalue");
	assertf(fabs(res.eigenvalues[1] - (-1.0)) < EPSILON, "wrong eigenvalue");
	assertf(fabs(res.eigenvalues[2] - 8.0) < EPSILON, "wrong eigenvalue");

	eigenvectors_free(res);
}

int main() {
	start_test(TEST);

	TEST_jacobi();

	end_test(TEST);
}
