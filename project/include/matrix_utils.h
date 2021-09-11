#ifndef PROJECT_MATRIX_UTILS_H
#define PROJECT_MATRIX_UTILS_H

#include "vector_utils.h"

/*
 *
 * MATRIX UTILS
 *
 */

typedef struct matrix {
	vector** rows;
	/*Number of columns.*/
	int n;
	/*Number of rows.*/
	int m;
	/*Size of rows array.*/
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

void matrix_swap(matrix* mat, int i, int j);

void matrix_slice(matrix* mat, int k);

void matrix_normalize_rows (matrix* mat);

#endif
