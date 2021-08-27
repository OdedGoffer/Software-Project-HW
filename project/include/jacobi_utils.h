#ifndef PROJECT_JACOBI_H
#define PROJECT_JACOBI_H

#include "../include/matrix_utils.h"

typedef struct ituple {
	int i;
	int j;
} ituple;

ituple get_largest_off_i_j(matrix* mat);

matrix* get_P(matrix* A);

#endif
