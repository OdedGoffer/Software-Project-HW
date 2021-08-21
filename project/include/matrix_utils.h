#ifndef PROJECT_MATRIX_UTILS_H
#define PROJECT_MATRIX_UTILS_H

#include "vector_utils.h"

typedef struct matrix {
	vector** rows;
	/*cols*/
	int n;
	/*rows*/
	int m;
} matrix;

matrix* matrix_init(int n, int m);

void matrix_free(matrix* mat);

#endif
