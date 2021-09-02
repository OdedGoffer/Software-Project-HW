#ifndef PROJECT_JACOBI_UTILS_H
#define PROJECT_JACOBI_UTILS_H

#include "../include/matrix_utils.h"

typedef struct ituple {
	int i;
	int j;
} ituple;

ituple get_largest_off_i_j(matrix* mat);

matrix* get_P(matrix* A);

matrix* get_A_tag(matrix* A, matrix* P);

#endif
