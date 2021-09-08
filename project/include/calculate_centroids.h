#ifndef SPKMEANS_C_CALCULATE_CENTROIDS_H
#define SPKMEANS_C_CALCULATE_CENTROIDS_H

#include "../include/matrix_utils.h"

/* Calculates centroids given M vectors and their respective clusters. 'centroids_arr'
 * represents to what cluster each vector in 'vectors' belongs. For example, if
 * vectors[4] == 2, this represents that row 4 in 'vectors' belongs to cluster number
 * 2.*/
matrix* calculate_centroids(int* centroids_arr, matrix* vectors, int K);

#endif
