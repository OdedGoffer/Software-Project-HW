#include "test_utils.h"
#include <stdio.h>
#include <assert.h>
#include "../include/vector_utils.h"

#define TEST "vector_utils"

void TEST_vector_init() {
	double vals[] = {1,2,3,4};
	int N = 4;
	vector* v;

	v = vector_init(vals, N);
	assertf(v != NULL, "new vector is NULL");
	assertf(v->values == vals, "wrong vector values");
	assertf(v->size == N, "vector has wrong size");

	vector_free(v);
}

int main() {
	start_test(TEST);

	TEST_vector_init();

	end_test(TEST);
}
