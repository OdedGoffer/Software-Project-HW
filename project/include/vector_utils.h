#ifndef PROJECT_VECTOR_UTILS_H
#define PROJECT_VECTOR_UTILS_H

typedef struct vector {
    double* vals;
    int N;
} vector;

vector* vector_init(double* vals, int N);

#endif //PROJECT_VECTOR_UTILS_H
