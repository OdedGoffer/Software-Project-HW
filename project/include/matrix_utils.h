#ifndef PROJECT_MATRIX_UTILS_H
#define PROJECT_MATRIX_UTILS_H

#include "vector_utils.h"

typedef struct matrix {
	vector** rows;
	/*cols*/
	int n;
	/*rows*/
	int m;
	/*size of rows array*/
	int row_cap;
} matrix;

matrix* matrix_init(int n, int m);

void matrix_free(matrix* mat);

void matrix_add_row(matrix* mat, vector* row);

matrix* matrix_copy(matrix* mat);

void matrix_print(matrix* mat);

#endif
