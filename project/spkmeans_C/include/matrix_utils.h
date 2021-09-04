#ifndef PROJECT_MATRIX_UTILS_H
#define PROJECT_MATRIX_UTILS_H

#include "vector_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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

/*Raises the value of the diagonal of mat by the power of a*/
void matrix_diagonal_pow(matrix* mat, double a);

/*Returns a pointer to the Identity matrix of size n*/
matrix* matrix_eye(int n);

/*Returns A - B*/
matrix* matrix_subtract(matrix* A, matrix* B);

/*Returns A * B*/
matrix* matrix_mult(matrix* A, matrix* B);

double matrix_off(matrix* mat);

void matrix_set(int i, int j, matrix* mat, double val);

double matrix_get(int i, int j, matrix* mat);

matrix* matrix_transpose(matrix* mat);

#endif
