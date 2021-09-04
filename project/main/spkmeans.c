#include "../test/test_utils/test_utils.h"
#include <stdio.h>
#include "../include/parse_file.h"

#define TEST "parse_file"

void TEST_parse_file() {
	char filename[] = "/Users/amitaig/Documents/Computer Science/Software-Project-HW/project/test/test_utils/output_1.txt";
	matrix* res;

	res = read_csv(filename);
	matrix_print(res);

	matrix_free(res);
}

int main() {
	start_test(TEST);

	TEST_parse_file();

	end_test(TEST);
}
