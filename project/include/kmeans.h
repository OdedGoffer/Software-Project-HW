#ifndef PROJECT_KMEANS_H
#define PROJECT_KMEANS_H
#include "../include/matrix_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/matrix_utils.h"

/* Classic K-means algorithm.*/
matrix* kmeans(matrix* inpt, int K);

#endif
