#include "test_utils/test_utils.h"
#include "../include/calculate_centroids.h"

#define TEST "calculate_centroids"
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

void TEST_calculate_centroids() {
	matrix *mat, *res;
	int centroids[3] = {0, 0, 1};

	mat = create_matrix();

	res = calculate_centroids(mat, centroids, 3);

	assertf(fabs(res->rows[0]->values[0] - 2.5) < EPSILON, "wrong centroid value");
	assertf(vector_dist(mat->rows[2], res->rows[1]) < EPSILON, "wrong centroid value");

	matrix_free(res);
	matrix_free(mat);

}

int main() {
	start_test(TEST);

	TEST_calculate_centroids();

	end_test(TEST);
}
