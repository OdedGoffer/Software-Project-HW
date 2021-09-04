#include "test_utils/test_utils.h"
#include "../include/parse_file.h"
#include "../include/kmeans.h"

#define TEST "kmeans"
#define EPSILON 0.001

void TEST_kmeans() {
	char input_filename[] = "test/test_utils/input_1.txt";
	char expected_filename[] = "test/test_utils/output_1.txt";
	matrix *input, *expected, *res;
	int i;

	input = read_csv(input_filename);
	expected = read_csv(expected_filename);

	res = kmeans(input, 3);

	for (i = 0; i < 3; i++) {
		assertf(vector_dist(res->rows[i], expected->rows[i]) < EPSILON, "wrong output value");
	}

	matrix_free(input);
	matrix_free(expected);
	matrix_free(res);
}

int main() {
	start_test(TEST);

	TEST_kmeans();

	end_test(TEST);
}
