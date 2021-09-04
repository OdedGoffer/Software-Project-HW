#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/matrix_utils.h"
#include "../include/kmeans.h"

#define MAX_ITER 200

struct vector_list;
struct S;

/* Centroid struct. Each centroid has a vector indicating
 * its center and a list of vectors representing all
 * vectors that belong to it.*/
struct S {
	struct vector_list* vectors;
	struct vector* center;
};

/*Vector-list struct. Each list element holds a pointer to a vector as well as the associated set, S.*/
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

/* Initiate a centroid with a center at vector element e.*/
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

/*Detach vector element e from its current centroid.*/
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

/*Add vector element e to cluster. This DOES NOT update the center of the cluster.*/
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

/*Returns the closest cluster to vector v.*/
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
			closest_clust = clusters;
			min_dist = vector_dist(v, center);
		}
	}
	return closest_clust;
}

/* Classic K-means algorithm.*/
matrix* kmeans(matrix* inpt, int K) {
	S *clusters, *min_S;
	vector_list* vectors;
	int iter = 0, change = 0, i, n, m;
	matrix* centroids;
	assert(inpt);

	vectors = vectors_init(inpt);
	clusters = clusters_init(vectors, K);
	n = inpt->n;
	m = inpt->m;

	while (iter < MAX_ITER) {
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
