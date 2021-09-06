#include "test_utils/test_utils.h"
#include "../include/vector_utils.h"

#define TEST "vector_utils"

void TEST_vector_init() {
	double nums[] = {1, 2, 3, 4};
	double* vals;
	int N = 4;
	vector* v;

	vals = (double*) malloc(N * sizeof(double));
	memcpy(vals, nums, N * sizeof(double));
	v = vector_init(vals, N);
	assertf(v != NULL, "new vector is NULL");
	assertf(v->values == vals, "wrong vector values");
	assertf(v->size == N, "vector has wrong size");

	vector_free(v);
}

void TEST_vector_init_zero() {
	int N =4;
	vector* v;

	v = vector_init_zero(N);
	assertf(v != NULL, "vector is NULL");
	assertf(v->values[0] == 0, "non zero values");

	vector_free(v);
}

void TEST_vector_add() {
	double nums1[] = {1, 1, 1, 1};
	double nums2[] = {0, 1, 2, 3};
	double* vals1;
	double* vals2;
	int N = 4;
	vector* v1;
	vector* v2;

	vals1 = (double*) malloc(N * sizeof(double));
	vals2 = (double*) malloc(N * sizeof(double));
	memcpy(vals1, nums1, N * sizeof(double));
	memcpy(vals2, nums2, N * sizeof(double));
	v1 = vector_init(vals1, N);
	v2 = vector_init(vals2, N);

	vector_add(v1, v2);

	assertf(v1->values[0] == 1, "wrong values after add");
	assertf(v1->values[1] == 2, "wrong values after add");
	assertf(v1->values[2] == 3, "wrong values after add");
	assertf(v1->values[3] == 4, "wrong values after add");

	vector_free(v1);
	vector_free(v2);

}

void TEST_vector_copy() {
	double nums[] = {1, 2, 3, 4};
	double* vals;
	int N = 4;
	vector* v1;
	vector* v2;

	vals = (double*) malloc(N * sizeof(double));
	memcpy(vals, nums, N * sizeof(double));
	v1 = vector_init(vals, N);
	v2 = vector_copy(v1);

	assertf(v1 != v2, "vectors are equal");

	v1->values[0] = 9;
	assertf(v2->values[0] == 1, "wrong values for v2");

	vector_free(v1);
	vector_free(v2);
}

void TEST_vector_dist() {
	double nums1[] = {1, 0, 0, 0};
	double nums2[] = {0, 1, 0, 0};
	double* vals1;
	double* vals2;
	double dist;
	int N = 4;
	vector* v1;
	vector* v2;

	vals1 = (double*) malloc(N * sizeof(double));
	vals2 = (double*) malloc(N * sizeof(double));
	memcpy(vals1, nums1, N * sizeof(double));
	memcpy(vals2, nums2, N * sizeof(double));
	v1 = vector_init(vals1, N);
	v2 = vector_init(vals2, N);

	dist = vector_dist(v1, v2);
	assertf(dist == sqrt(2), "wrong distance");

	dist = vector_dist(v1, v1);
	assertf(dist == 0, "self distance is not 0");

	vector_free(v1);
	vector_free(v2);
}

void TEST_vector_values_sum() {
	double nums[] = {1, 2, 3, 4};
	double* vals;
	int N = 4;
	vector* v;
	double result;

	vals = (double*) malloc(N * sizeof(double));
	memcpy(vals, nums, N * sizeof(double));
	v = vector_init(vals, N);

	result = vector_values_sum(v);

	assertf(result == 10, "sum is incorrect");

	vector_free(v);
}

int main() {
	start_test(TEST);

	TEST_vector_init();
	TEST_vector_init_zero();
	TEST_vector_add();
	TEST_vector_copy();
	TEST_vector_dist();

	end_test(TEST);
}
