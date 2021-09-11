#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "spkmeans.h"

/*
 *
 * LOGGER
 *
 */

void invalid_input() {
	printf("Invalid Input!\n");
	exit(EXIT_FAILURE);
}

void error_occured() {
	printf("An Error Has Occured\n");
	exit(EXIT_FAILURE);
}

/*
 *
 * VECTOR UTILS
 *
 */

/* Creates a new vector object.*/
vector* vector_init(double* values, int size) {
	vector* v;

	v = (vector*) malloc(sizeof(vector));
	assert(v != NULL);
	v->values = values;
	v->size = size;
	return v;
}

/* Creates a new vector object with zero values.*/
vector* vector_init_zero(int size) {
	vector* v;

	v = (vector*)malloc(sizeof(vector));
	assert(v != NULL);
	v->values = (double*) calloc(size, sizeof(double));
	v->size = size;
	return v;
}

void vector_free(vector* v) {
	free(v->values);
	free(v);
}

/* Adds v2 to v1 in place.*/
void vector_add(vector* v1, vector* v2) {
	int i;
	assert(v1 != NULL && v2 != NULL);
	assert(v1->size == v2->size);

	for (i = 0; i < v1->size; i++) {
		v1->values[i] += v2->values[i];
	}
}

/* Divide all values of v by c in place.*/
void vector_divide(vector* v, double c) {
	int i;
	assert (v);
	assert(c);

	for (i=0; i<v->size; i++) {
		v->values[i] = v->values[i]/c;
	}
}

/* Make v equal to zero vector in place.*/
void vector_zero(vector* v) {
	int i;
	assert(v);

	for(i=0; i<v->size; i++) {
		v->values[i] = 0.0;
	}
}

/* Creates a deep copy of v.*/
vector* vector_copy(vector* v) {
	double* vals_cpy;
	assert(v != NULL);

	vals_cpy = (double*) malloc(sizeof(double) * v->size);
	assert(vals_cpy != NULL);
	memcpy(vals_cpy, v->values, sizeof(double) * v->size);

	return vector_init(vals_cpy, v->size);
}

/* Euclidean distance between vectors*/
double vector_dist(vector* v1, vector* v2) {
	double sum = 0;
	double diff;
	int i;
	assert(v1 != NULL && v2 != NULL);
	assert(v1->size == v2->size);

	for (i=0; i<v1->size; i++) {
		diff = v1->values[i] - v2->values[i];
		sum += pow(diff, 2);
	}

	return sqrt(sum);
}

void vector_print(vector* v) {
	int n;
	int i;
	assert(v != NULL);

	n = v->size;
	for (i = 0; i < n-1; i++){
		printf("%.4f,", v->values[i]);
	}
	printf("%.4f\n", v->values[n-1]);
}

/* Returns the sum of all values in vector object.*/
double vector_values_sum(vector* v) {
	double sum = 0;
	int i;

	for (i=0; i<v->size; i++) {
		sum += v->values[i];
	}

	return sum;
}

/*
 *
 * MATRIX UTILS
 *
 */

matrix* matrix_init(int n, int m) {
	matrix* mat;
	vector** rows;
	int i;
	assert(!(n<0 || m<0));

	rows = NULL;
	mat = (matrix*) malloc(sizeof(matrix));
	assert(mat != NULL);
	if (m != 0) {
		rows = (vector**) malloc(m * sizeof(vector*));
		assert(rows != NULL);
	}
	mat->n = n;
	mat->m = m;
	mat->rows = rows;
	mat->row_cap = m;

	for (i = 0; i < m; i++) {
		rows[i] = vector_init_zero(n);
	}

	return mat;
}

void matrix_free(matrix* mat) {
	int i;

	if (mat == NULL) return;

	for (i=0; i<mat->m; i++) {
		vector_free(mat->rows[i]);
	}
	free(mat->rows);
	free(mat);
}

/* Adds a new row to the end of the matrix.*/
void matrix_add_row(matrix* mat, vector* row) {
	vector** new_rows;
	int m;
	assert(mat != NULL && row != NULL);
	assert(mat->n == row->size);
	m = mat->m;

	if (mat->row_cap == m) {
		if (m == 0) {
			m = 1;
		}
		new_rows = (vector**) calloc((m * 2), sizeof(vector*));
		assert(new_rows != NULL);
		memcpy(new_rows, mat->rows, (mat->m) * sizeof(vector*));
		free(mat->rows);
		mat->rows = new_rows;
		mat->row_cap = 2 * m;
	}

	mat->rows[mat->m] = row;
	mat->m++;
}

/* Returns a deep copy of matrix 'mat'.*/
matrix* matrix_copy(matrix* mat) {
	matrix* new_mat;
	int n;
	int m;
	int i;
	assert(mat != NULL);

	n = mat->n;
	m = mat->m;
	new_mat = matrix_init(n, 0);

	for (i=0; i<m; i++) {
		matrix_add_row(new_mat, vector_copy(mat->rows[i]));
	}

	return new_mat;
}

void matrix_print(matrix* mat) {
	int i;
	assert(mat != NULL);

	for (i=0; i<mat->m; i++) {
		vector_print(mat->rows[i]);
	}
}

/*'mat' must be a square matrix with positive diagonal values.*/
void matrix_diagonal_pow(matrix* mat, double a) {
	double val;
	int i;
	assert(mat->m == mat->n);

	for (i=0; i<mat->m; i++) {
		val = mat->rows[i]->values[i];
		assert(val > 0);
		val = pow(val, fabs(a));
		if (a < 0) {
			val = 1.0/val;
		}
		mat->rows[i]->values[i] = val;
	}
}

/*Returns a pointer to the Identity matrix of size n*/
matrix* matrix_eye(int n) {
	matrix* mat;
	int i;
	assert(!(n <= 0));

	mat = matrix_init(n, n);
	assert(mat != NULL);

	for (i = 0; i < n; i++) {
		mat->rows[i]->values[i] = 1;
	}

	return mat;
}

/*Returns A - B. Returns a new matrix.*/
matrix* matrix_subtract(matrix* A, matrix* B) {
	matrix* ret;
	int i, j, n;

	n = A->n;
	ret = matrix_init(n, n);
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			ret->rows[i]->values[j] = A->rows[i]->values[j] - B->rows[i]->values[j];
		}
	}

	return ret;
}

/*Returns A * B. Returns a new matrix.*/
matrix* matrix_mult(matrix* A, matrix* B) {
	int i, j, k, n;
	double sum = 0;
	matrix* ret;

	n = A->n;
	ret = matrix_init(n, n);
	for (i=0; i<n; i++) {
		for (j=0; j<n; j++) {
			for (k=0; k<n; k++) {
				sum += A->rows[i]->values[k] * B->rows[k]->values[j];
			}
			ret->rows[i]->values[j] = sum;
			sum = 0;
		}
	}

	return ret;
}

/* Calculates off(mat)^2 as defined in Jacobi algorithm.*/
double matrix_off(matrix* mat) {
	int n, m, i, j;
	double sum = 0;
	assert(mat != NULL);

	n = mat->n;
	m = mat->m;
	assert(m == n);

	for (i=0; i<n; i++) {
		for (j=0; j<m; j++) {
			if (i == j) {
				continue;
			}
			sum += pow(mat->rows[j]->values[i], 2);
		}
	}
	return sum;
}

void matrix_set(int i, int j, matrix* mat, double val) {
	assert(i >= 0 && i < mat->m);
	assert(j >= 0 && j < mat->n);
	mat->rows[i]->values[j] = val;
}

double matrix_get(int i, int j, matrix* mat) {
	assert(i >= 0 && i < mat->m);
	assert(j >= 0 && i < mat->n);
	return mat->rows[i]->values[j];
}

matrix* matrix_transpose(matrix* mat) {
	matrix* mat_transpose;
	int n_old, m_old, n_new, m_new, i, j;
	double val;
	assert(mat);

	n_old = mat->n;
	m_old = mat->m;

	n_new = m_old;
	m_new = n_old;

	mat_transpose = matrix_init(n_new, m_new);

	for (i = 0; i < n_old; i++) {
		for (j = 0; j < m_old; j++) {
			val = mat->rows[j]->values[i];
			mat_transpose->rows[i]->values[j] = val;
		}
	}
	return mat_transpose;
}

/* Swap rows 'i' and 'j' of matrix 'mat'.*/
void matrix_swap(matrix* mat, int i, int j) {
	vector* tmp;

	tmp = mat->rows[i];
	mat->rows[i] = mat->rows[j];
	mat->rows[j] = tmp;
}

/* Returns a slice of matrix 'mat' containing only first 'k' rows.*/
void matrix_slice(matrix* mat, int k) {
	int i, m;
	vector** new_rows;
	vector** old_rows;

	old_rows = mat->rows;
	m = mat->m;
	for (i=k; i<m; i++) {
		vector_free(mat->rows[i]);
	}

	new_rows = (vector**) malloc(sizeof(vector*) * k);
	assert(new_rows);
	memcpy(new_rows, old_rows, sizeof(vector*) * k);
	mat->rows = new_rows;
	mat->m = k;
	mat->row_cap = k;
	free(old_rows);
}

/*
 *
 * WAM
 *
 */

matrix* WAM(matrix* X) {
	matrix* ret;
	int n, i, j;
	double* vals;
	double dist;
	vector* new_vec;

	n = X->m;
	ret = matrix_init(n, 0);

	for (i = 0; i < n; i++) {
		vals = (double*) malloc(n * sizeof(double));
		assert(vals);
		for (j = 0; j < n; j++) {
			if (i == j) {
				vals[j] = 0;
			} else {
				dist = vector_dist(X->rows[i], X->rows[j]) / (-2);
				vals[j] = exp(dist);
			}
		}
		new_vec = vector_init(vals, n);
		matrix_add_row(ret, new_vec);
	}

	return ret;
}

/*
 *
 * DDG
 *
 */

matrix* DDG(matrix* W) {
	matrix* ret;
	int m, i;
	double* vals;
	double sum;
	vector* vec;

	m = W->m;
	ret = matrix_init(m, 0);

	for (i=0; i<m; i++) {
		vals = (double*)calloc(m, sizeof(double));
		assert(vals);
		sum = vector_values_sum(W->rows[i]);
		vals[i] = sum;
		vec = vector_init(vals, m);
		matrix_add_row(ret, vec);
	}

	return ret;
}

/*
 *
 * LNORM
 *
 */

matrix* LNORM(matrix* W, matrix* D) {
	matrix *ret1, *ret2, *ret3, *eye;
	int n;

	n = W->n;
	eye = matrix_eye(n);
	matrix_diagonal_pow(D, -0.5);

	ret1 = matrix_mult(D, W);
	ret2 = matrix_mult(ret1, D);
	ret3 = matrix_subtract(eye, ret2);

	matrix_free(eye);
	matrix_free(ret1);
	matrix_free(ret2);
	return ret3;
}

/*
 *
 * JACOBI UTILS
 *
 */

/* Returns index (i,j) of largest off-center element of 'mat'.*/
ituple get_largest_off_i_j(matrix* mat) {
	double val = 0.0;
	double curr;
	int n, i, j;
	ituple res = {
			0,
			1,
	};
	assert(mat != NULL);

	n = mat->n;
	assert(n == mat->m);
	assert(n > 1);

	for (i = 0; i < n; i++) {
		for (j = i + 1; j < n; j++) {
			curr = fabs(matrix_get(i, j, mat));
			if (curr > val) {
				val = curr;
				res.i = i;
				res.j = j;
			}
		}
	}
	return res;
}

double get_theta(matrix* mat, int i, int j) {
	double Aij, Aii, Ajj;
	assert(mat != NULL);

	Aij = matrix_get(i, j, mat);
	Aii = matrix_get(i, i, mat);
	Ajj = matrix_get(j, j, mat);

	return (Ajj - Aii) / (2 * Aij);
}

double sign(double x) {
	if (x < 0) {
		return -1.0;
	}
	return 1.0;
}

double get_t(double theta) {
	double theta_sqrd, abs_theta, sign_theta;

	theta_sqrd = pow(theta, 2);
	abs_theta = fabs(theta);
	sign_theta = sign(theta);

	return (sign_theta) / (abs_theta + sqrt(1 + theta_sqrd));

	/* 		    sign(theta)
	* 	----------------------------
	* 	abs(theta) + sqrt(1 + theta^2)
	*/
}

double get_c(double t) {
	double t_sqrd;

	t_sqrd = pow(t, 2);
	return 1 / (sqrt(1 + t_sqrd));
}

double get_s(double t, double c) {
	return c * t;
}

/* Calculate matrix 'P' and update A = A' as defined in jacobi algorithm.*/
matrix* get_P_and_update_A(matrix* A) {
	double theta, c, t, s, val;
	ituple ij;
	int n, i, j, p;
	matrix *P, *A_tmp;
	assert(A != NULL);

	n = A->n;
	assert(n == A->m);
	P = matrix_eye(n);

	/* Special case: n == 1, there is nothing to do, and so we return I. */
	if (n == 1) return P;

	ij = get_largest_off_i_j(A);
	i = ij.i;
	j = ij.j;

	/* Special case: Aij == 0, meaning A is diagonal, and so we return I. */
	if (!matrix_get(i, j, A)) return P;

	theta = get_theta(A, i, j);
	t = get_t(theta);
	c = get_c(t);
	s = get_s(c, t);

	matrix_set(i, i, P, c);
	matrix_set(j, j, P, c);
	matrix_set(i, j, P, s);
	matrix_set(j, i, P, -s);

	A_tmp = matrix_copy(A);

	for (p = 0; p < n; p++) {
		if (p == i || p == j) continue;

		val = (c * matrix_get(p, i, A_tmp)) - (s * matrix_get(p, j, A_tmp));
		matrix_set(p, i, A, val);
		matrix_set(i, p, A, val);

		val = (c * matrix_get(p, j, A_tmp)) + (s * matrix_get(p, i, A_tmp));
		matrix_set(p, j, A, val);
		matrix_set(j, p, A, val);
	}

	val = (pow(c, 2) * matrix_get(i, i, A_tmp)) + (pow(s, 2) * matrix_get(j, j, A_tmp)) - (2 * s * c * matrix_get(i, j, A_tmp));
	matrix_set(i, i, A, val);

	val = (pow(s, 2) * matrix_get(i, i, A_tmp)) + (pow(c, 2) * matrix_get(j, j, A_tmp)) + (2 * s * c * matrix_get(i, j, A_tmp));
	matrix_set(j, j, A, val);

	matrix_set(i, j, A, 0);
	matrix_set(j, i, A, 0);

	matrix_free(A_tmp);

	return P;
}

/*
 *
 * JACOBI
 *
 */

#define JACOBI_MAX_ITER 100
#define EPSILON 0.001

int check_convergence(matrix* A, matrix* A_tag) {
	double A_off, A_tag_off;
	assert((A) && (A_tag));

	A_off = matrix_off(A);
	A_tag_off = matrix_off(A_tag);
	if(fabs(A_off - A_tag_off) < EPSILON) return 1;
	return 0;
}

/* Returns (P_mult * P) */
matrix* multiply_P(matrix* P, matrix* P_mult) {
	matrix *P_mult_old;
	assert(P);

	if (P_mult != NULL) {
		P_mult_old = P_mult;
		P_mult = matrix_mult(P_mult_old, P);
		matrix_free(P_mult_old);
		return P_mult;
	}

	return matrix_copy(P);
}

/* Returns diagonal elements of 'A' as an array.*/
double* diagonal_to_array(matrix* A) {
	double* res;
	int n, i;
	assert(A);

	n = A->n;
	assert(n == A->m);

	res = (double*)malloc(n*sizeof(double));
	assert(res);
	for (i = 0; i < n; i++) {
		res[i] = A->rows[i]->values[i];
	}
	return res;
}

vectors_values_pair jacobi(matrix* A) {
	int i;
	int done = 0;
	matrix* A_tag, *P, *P_mult;
	vectors_values_pair res;

	assert(A);
	P_mult = NULL;

	for (i = 0; i < JACOBI_MAX_ITER; i++) {
		A_tag = matrix_copy(A);
		P = get_P_and_update_A(A);
		P_mult = multiply_P(P, P_mult);

		if (check_convergence(A, A_tag)) done = 1;

		matrix_free(P);
		matrix_free(A_tag);

		if (done == 1) break;
	}

	res.eigenvectors = matrix_transpose(P_mult);
	res.eigenvalues = diagonal_to_array(A);
	res.n = A->n;
	matrix_free(P_mult);
	matrix_free(A);

	return res;
}

void eigenvectors_free(vectors_values_pair vvp) {
	matrix_free(vvp.eigenvectors);
	free(vvp.eigenvalues);
}

/*
 *
 * EIGENGAP HEURISTIC
 *
 */

/* Stable selection-sort on array 'a'. In addition, we reorder the rows of matrix
 * 'mat' using the same permutation as selection-sort on 'a'. */
void stableSelectionSort(matrix* mat, double* a, int n) {
	int i, j, min;
	double key;
	vector* vec;

	for (i = 0; i < n - 1; i++) {
        min = i;

        for (j = i + 1; j < n; j++) {

        	if (a[min] > a[j]) {
        		min = j;
        	}

            key = a[min];
            vec = mat->rows[min];

            while (min > i) {
            	a[min] = a[min - 1];
            	matrix_swap(mat, min, min-1);
                min--;
            }
            a[i] = key;
            mat->rows[i] = vec;
        }
    }
}

/* Calculate K values from eigenvalues using eigengap-heuristic.
 * Retrun K if K != 0.*/
int get_k(double* eigenvalues, int k, int n) {
	double delta;
	double max_diff = 0;
	int i, index = 1;

	if (k != 0) {
		return k;
	}

	for (i=0; i<n-1; i++) {
		delta = eigenvalues[i+1] - eigenvalues[i];
		if (delta > max_diff) {
			max_diff = delta;
			index = i+1;
		}
	}

	return index;
}

/*Transpose and normalize rows of matrix u.*/
matrix* get_T(matrix* U) {
	matrix *T, *M;
	int i, j, n, m;
	double norm, val;
	vector* zero;

	M = matrix_transpose(U);
	matrix_free(U);

	n = M->n;
	m = M->m;
	T = matrix_init(n, m);
	zero = vector_init_zero(n);

	for (i=0; i<m; i++) {
		norm = vector_dist(M->rows[i], zero);
		if (norm == 0) error_occured();
		for (j=0; j<n; j++) {
			val = M->rows[i]->values[j] / norm;
			matrix_set(i, j, T, val);
		}
	}

	vector_free(zero);
	matrix_free(M);
	return T;
}

vectors_k_pair eigengap_heuristic(vectors_values_pair pair, int k) {
	vectors_k_pair verctors_pair;
	int n;

	n = pair.n;

	stableSelectionSort(pair.eigenvectors, pair.eigenvalues, n);

	k = get_k(pair.eigenvalues, k, n);
	matrix_slice(pair.eigenvectors, k);

	verctors_pair.vectors = get_T(pair.eigenvectors);
	verctors_pair.k = k;

	return verctors_pair;
}

/*
 *
 * KMEANS
 *
 */

#define KMEANS_MAX_ITER 300

struct vector_list;
struct S;

/* Centroid struct. Each centroid has a vector indicating
 * its center and a list of vectors representing all
 * vectors that belong to it.*/
struct S {
	struct vector_list* vectors;
	struct vector* center;
};

/*Vector-list struct. Each list element holds a pointer to a vector as well as the associated set, 'S'.*/
struct vector_list {
	vector* vector;
	struct S* S;
	struct vector_list* next;
	struct vector_list* prev;
};

typedef struct vector_list vector_list;
typedef struct S S;

/*Initiate and array of vector elements from input matrix.*/
vector_list* vectors_init(matrix* inpt) {
	vector_list* vectors;
	int m, i;
	assert(inpt);

	m = inpt->m;
	vectors = (vector_list*) malloc(m * sizeof(vector_list));
	assert(vectors);

	for (i = 0; i < m; i++) {
		vectors[i].vector = inpt->rows[i];
		vectors[i].S = NULL;
		vectors[i].next = NULL;
		vectors[i].prev = NULL;
	}

	return vectors;
}

/* Initiate a centroid with a center at vector-element 'e'.*/
void S_init(vector_list* e, S* S) {
	assert(e);
	assert(S);

	S->vectors = e;
	e->S = S;
	S->center = vector_copy(e->vector);
}

/*Initiate K empty centroids with centers at the first K vectors.*/
S* clusters_init(vector_list* vectors, int K) {
	S* clusters;
	int i;
	assert(vectors);

	clusters = (S*) malloc(K * sizeof(S));
	assert(clusters);

	for (i = 0; i < K; i++) {
		if (vectors[i].vector == NULL) {
			printf("%s\n", "Not enough vectors!");
			exit(EXIT_FAILURE);
		}
		S_init(&vectors[i], &clusters[i]);
	}

	return clusters;
}

/*Recalculate the center of centroid S, as defined in K-means algorithm.*/
void recenter(S* S) {
	vector_list* current;
	double n = 0.0;
	assert(S);

	current = S->vectors;
	vector_zero(S->center);

	if (current == NULL) return;

	while (current != NULL) {
		n += 1.0;
		vector_add(S->center, current->vector);
		current = current->next;
	}

	vector_divide(S->center, n);
}

/*Detach vector-element 'e' from its current centroid.*/
void remove_S(vector_list* e) {
	vector_list *next, *prev;
	S* S;
	assert(e);

	S = e->S;
	if (S == NULL) return;
	e->S = NULL;

	prev = e->prev;
	next = e->next;

	if (prev == NULL && next == NULL) {
		S->vectors = NULL;
	} else if (prev == NULL) {
		S->vectors = next;
		next->prev = NULL;
	} else if (next == NULL) {
		prev->next = NULL;
	} else {
		next->prev = prev;
		prev->next = next;
	}
	e->next = NULL;
	e->prev = NULL;
}

/*Add vector-element 'e' to cluster 'S'. This DOES NOT update the center of the cluster.*/
void add_S(S* S, vector_list* e) {
	assert(S);
	assert(e);

	remove_S(e);
	e->S = S;
	e->next = S->vectors;
	if (S->vectors != NULL) {
		(S->vectors)->prev = e;
	}
	S->vectors = e;
}

/*Returns the closest cluster to vector 'v'.*/
S* closest_clust(vector* v, S* clusters, int K) {
	S* closest_clust;
	double min_dist;
	vector* center;
	int i;
	assert(v);
	assert(clusters);

	center = clusters[0].center;
	min_dist = vector_dist(v, center);
	closest_clust = clusters;
	for (i = 1; i < K; i++) {
		center = clusters[i].center;
		if (min_dist > vector_dist(v, center)) {
			closest_clust = &clusters[i];
			min_dist = vector_dist(v, center);
		}
	}
	return closest_clust;
}

/*Classic K-means algorithm.*/
matrix* kmeans(matrix* inpt, int K) {
	S *clusters, *min_S;
	vector_list* vectors;
	matrix* centroids;
	int iter = 0, change, i, m, n;
	assert(inpt);

	vectors = vectors_init(inpt);
	clusters = clusters_init(vectors, K);
	n = inpt->n;
	m = inpt->m;

	while (iter < KMEANS_MAX_ITER) {
		iter++;
		change = 0;
		for (i = 0; i < m; i++) {
			min_S = closest_clust(vectors[i].vector, clusters, K);
			if (vectors[i].S != min_S) {
				change = 1;
				add_S(min_S, &vectors[i]);
			}
		}

		if (change == 0) break;

		for (i = 0; i < K; i++) recenter(&clusters[i]);
	}

	centroids = matrix_init(n, 0);
	for (i = 0; i < K; i++) matrix_add_row(centroids, clusters[i].center);

	free(vectors);
	free(clusters);
	return centroids;
}

/*
 *
 * PARSE FILE
 *
 */

/* Read csv file at path 'filename' into a matrix object and return.*/
matrix* read_csv(char* filename) {
	FILE* fp;
	int result, i = 0, n = 0;
	double val, values[256];
	char c;
	vector *v=NULL, *v_new;
	matrix* res;

	fp = fopen(filename, "r");
	if (!fp) error_occured();

	while(fscanf(fp, "%lf%c", &val, &c) == 2) {
		if (c != ',') break;
		n++;
	}
	n++;

	res = matrix_init(n, 0);
	rewind(fp);

	while (fscanf(fp, "%lf", &val) == 1) {
		values[i] = val;
		i++;
		if (i == n) {
			v = vector_init(values, n);
			v_new = vector_copy(v);
			free(v);
			matrix_add_row(res, v_new);
			i = 0;
		}
		if (!fscanf(fp, "%c", &c)) {
			break;
		}
	}

	result = fclose(fp);
	if (result == EOF) error_occured();

	return res;
}

/*
 *######################
 * MAIN
 * #####################
 */

typedef enum {
	_SPK,
	_WAM,
	_DDG,
	_LNORM,
	_JACOBI
} goal;

typedef struct {
	goal goal;
	int K;
	char* filename;
} args;

void print_eigenvalues(vectors_values_pair vvp) {
	int i;

	for (i = 0; i < vvp.n - 1; i++) {
		printf("%.4f,", vvp.eigenvalues[i]);
	}
	printf("%.4f\n", vvp.eigenvalues[vvp.n - 1]);
}

args parse_cmd(int argc, char* argv[]) {
	args args;
	char* goal_str;
	int K;

	if (argc != 4) invalid_input();

	goal_str = argv[2];

	if (strcmp(goal_str, "spk") == 0) {
		args.goal = _SPK;
	} else if (strcmp(goal_str, "wam") == 0) {
		args.goal = _WAM;
	} else if (strcmp(goal_str, "ddg") == 0) {
		args.goal = _DDG;
	} else if (strcmp(goal_str, "lnorm") == 0) {
		args.goal = _LNORM;
	} else if (strcmp(goal_str, "jacobi") == 0) {
		args.goal = _JACOBI;
	} else invalid_input();

	K = atoi(argv[1]);

	if (K < 0) invalid_input();

	args.K = K;

	args.filename = argv[3];

	return args;
}

void do_spkmeans(matrix* input, int K) {
	matrix *W, *D, *Lnorm, *T, *centroids;
	vectors_values_pair pair;
	vectors_k_pair vectors_pair;

	W = WAM(input);

	D = DDG(W);

	Lnorm = LNORM(W, D);

	pair = jacobi(Lnorm);

	vectors_pair = eigengap_heuristic(pair, K);

	T = vectors_pair.vectors;
	K = vectors_pair.k;

	centroids = kmeans(T, K);

	matrix_free(W);
	matrix_free(D);
	matrix_free(T);
	free(pair.eigenvalues);

	matrix_print(centroids);
	matrix_free(centroids);
	matrix_free(input);
}

void do_wam(matrix* input) {
	matrix* res;

	res = WAM(input);
	matrix_print(res);
	matrix_free(res);
	matrix_free(input);
}

void do_ddg(matrix* input) {
	matrix *W, *res;

	W = WAM(input);
	res = DDG(W);
	matrix_print(res);
	matrix_free(W);
	matrix_free(res);
	matrix_free(input);
}

void do_lnorm(matrix* input) {
	matrix *res, *W, *D;

	W = WAM(input);
	D = DDG(W);
	res = LNORM(W, D);

	matrix_print(res);

	matrix_free(W);
	matrix_free(D);
	matrix_free(res);
	matrix_free(input);
}

void do_jacobi(matrix* input) {
	vectors_values_pair pair;

	if (input->n != input->m) invalid_input();

	pair = jacobi(input);

	print_eigenvalues(pair);
	matrix_print(pair.eigenvectors);

	eigenvectors_free(pair);
}

int main(int argc, char* argv[]) {
	args args;
	matrix* input;

	args = parse_cmd(argc, argv);
	input = read_csv(args.filename);

	if (args.K >= input->m) invalid_input();

	switch (args.goal) {
		case _SPK:
			do_spkmeans(input, args.K);
			break;
		case _WAM:
			do_wam(input);
			break;
		case _DDG:
			do_ddg(input);
			break;
		case _LNORM:
			do_lnorm(input);
			break;
		case _JACOBI:
			do_jacobi(input);
			break;
	}

	return 0;
}
