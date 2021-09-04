#include "test_utils/test_utils.h"
#include <stdio.h>
#include "../include/parse_file.h"

#define TEST "parse_file"
#define EPSILON 0.001

void TEST_read_csv() {
	char filename[] = "test/test_utils/output_1.txt";
	matrix* res;

	res = read_csv(filename);

	assertf(fabs(matrix_get(0, 0, res) - (-4.3251)) < EPSILON, "wrong value");
	assertf(fabs(matrix_get(2, 2, res) - (-7.3358)) < EPSILON, "wrong value");

	matrix_free(res);
}

int main() {
	start_test(TEST);

	TEST_read_csv();

	end_test(TEST);
}
