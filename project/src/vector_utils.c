#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../include/vector_utils.h"

vector* vector_init(double* values, int size) {
	vector* v;

	v = (vector*) malloc(sizeof(vector));
	assert(v != NULL);
	v->values = values;
	v->size = size;
	return v;
}

void vector_free(vector* v) {
	free(v);
}

void vector_add(vector* v1, vector* v2) {
	int i;
	assert(v1 != NULL && v2 != NULL);
	assert(v1->size == v2->size);

	for (i = 0; i < v1->size; i++) {
		v1->values[i] += v2->values[i];
	}
}

vector* vector_copy(vector* v) {
	double* vals_cpy;
	assert(v != NULL);

	vals_cpy = (double*) malloc(sizeof(double) * v->size);
	memcpy(vals_cpy, v->values, sizeof(double) * v->size);

	return vector_init(vals_cpy, v->size);
}
