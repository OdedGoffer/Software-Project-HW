#include "../include/calculate_centroids.h"
#include <assert.h>
#include <stdlib.h>

/*Calculates the center of matrix 'cluster' as defined in K-means algorithm.*/
vector* calculate_center(matrix* cluster) {
	vector* center;
	int i, n, m;
	assert(cluster);

	n = cluster->n;
	m = cluster->m;
	center = vector_init_zero(n);

	/*Special case: centroid is empty. TODO: Ask what to do here*/
	if (m == 0) return center;

	for (i = 0; i < m; i++) {
		vector_add(center, cluster->rows[i]);
	}

	vector_divide(center, (double)m);
	return center;
}

/* Calculates centroids given M vectors and their respective clusters. 'centroids_arr'
 * represents to what cluster each vector in 'vectors' belongs. For example, if
 * vectors[4] == 2, this represents that row 4 in 'vectors' belongs to cluster number
 * 2.*/
matrix* calculate_centroids(matrix* vectors, int* centroids_arr, int K) {
	matrix *centroids, *cluster;
	vector *v, *center;
	int i, j, n, m;
	assert(vectors);
	assert(centroids_arr);

	n = vectors->n;
	m = vectors->m;
	centroids = matrix_init(n, 0);

	for (i = 0; i < K; i++) {
		cluster = matrix_init(n, 0);
		for (j = 0; j < m; j++) {
			if (centroids_arr[j] == i) {
				v = vector_copy(vectors->rows[j]);
				matrix_add_row(cluster, v);
			}
		}
		center = calculate_center(cluster);
		matrix_add_row(centroids, center);
		free(cluster);
	}
	return centroids;
}
