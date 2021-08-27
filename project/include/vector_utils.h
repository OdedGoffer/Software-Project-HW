#ifndef PROJECT_VECTOR_UTILS_H
#define PROJECT_VECTOR_UTILS_H

typedef struct vector {
	double* values;
	int size;
} vector;

/*Creates a new vector object.*/
vector* vector_init(double* values, int size);

vector* vector_init_zero(int size);

void vector_free(vector* v);

/*Adds v2 to v1 in place.*/
void vector_add(vector* v1, vector* v2);

/*Creates a deep copy of v.*/
vector* vector_copy(vector* v);

/*Euclidean distance between vectors*/
double vector_dist(vector* v1, vector* v2);

void vector_print(vector* v);

/*Returns the sum of all values in vector->values*/
double vector_values_sum(vector* v);

#endif
