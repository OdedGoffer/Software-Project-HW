#define PY_SSIZE_T_CLEAN
#include <Python/Python.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/*
 *
 * DEFINITIONS
 *
 */

#define JACOBI_MAX_ITER 100
#define KMEANS_MAX_ITER 300
#define EPSILON 0.1

/*
 *
 * STRUCT DEFINITIONS
 *
 */

typedef struct vector {
	double* values;
	int size;
} vector;

typedef struct matrix {
	vector** rows;
	/*Number of columns.*/
	int n;
	/*Number of rows.*/
	int m;
	/*Size of rows array.*/
	int row_cap;
} matrix;


/*Type 'ituple' is an integer tuple*/
typedef struct ituple {
	int i;
	int j;
} ituple;

/* Type 'vectors_values_pair' holds a matrix of eigenvectors and an
 * array of corresponding eigenvalues.*/
typedef struct vectors_values_pair {
	matrix* eigenvectors;
	double* eigenvalues;
	int n;
} vectors_values_pair;

/* Type 'vectors_k_pair' holds a matrix of normalized vectors and the
 * updates value of k (using Eigengap algorithm if required).*/
typedef struct vectors_k_pair {
	matrix* vectors;
	int k;
} vectors_k_pair;
