#ifndef PROJECT_JACOBI_UTILS_H
#define PROJECT_JACOBI_UTILS_H

#include "../include/matrix_utils.h"

/*
 *
 * JACOBI UTILS
 *
 */

/*Type 'ituple' is an integer tuple*/
typedef struct ituple {
	int i;
	int j;
} ituple;

/*Returns 'i', 'j'; indeces of largest off-center element in diagonal matrix.*/
ituple get_largest_off_i_j(matrix* mat);

/*Returns 'P'; transition matrix of A*/
matrix* get_P_and_update_A(matrix* A);

#endif
