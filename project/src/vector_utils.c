#include "../include/vector_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

vector* vector_init(double* values, int size) {
	vector* v;

	v = (vector*) malloc(sizeof(vector));
	assert(v != NULL);
	v->values = values;
	v->size = size;
	return v;
}

vector* vector_init_zero(int size) {
	vector* v;

	v = (vector*)malloc(sizeof(vector));
	assert(v != NULL);
	v->values = (double*) calloc(size, sizeof(double));
	v->size = size;
	return v;
}

void vector_free(vector* v) {
	free(v->values);
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

void vector_divide(vector* v, double c) {
	int i;
	assert (v);
	assert(c);

	for (i=0; i<v->size; i++) {
		v->values[i] = v->values[i]/c;
	}
}

void vector_zero(vector* v) {
	int i;
	assert(v);

	for(i=0; i<v->size; i++) {
		v->values[i] = 0.0;
	}
}

vector* vector_copy(vector* v) {
	double* vals_cpy;
	assert(v != NULL);

	vals_cpy = (double*) malloc(sizeof(double) * v->size);
	assert(vals_cpy != NULL);
	memcpy(vals_cpy, v->values, sizeof(double) * v->size);

	return vector_init(vals_cpy, v->size);
}

double vector_dist(vector* v1, vector* v2) {
	double sum = 0;
	double diff;
	int i;
	assert(v1 != NULL && v2 != NULL);
	assert(v1->size == v2->size);

	for (i=0; i<v1->size; i++) {
		diff = v1->values[i] - v2->values[i];
		sum += pow(diff, 2);
	}

	return sqrt(sum);
}

void vector_print(vector* v) {
	int n;
	int i;
	assert(v != NULL);

	n = v->size;
	for (i = 0; i < n-1; i++){
		printf("%.4f,", v->values[i]);
	}
	printf("%.4f\n", v->values[n-1]);
}

double vector_values_sum(vector* v) {
	double sum = 0;
	int i;

	for (i=0; i<v->size; i++) {
		sum += v->values[i];
	}

	return sum;
}

double vector_length(vector* v) {
	vector* zero;
	assert(v);

	zero = vector_init_zero(v->size);
	return vector_dist(v, zero);
}
