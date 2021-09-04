#ifndef PROJECT_Eigengap_Heuristic_H
#define PROJECT_Eigengap_Heuristic_H

#include "../include/matrix_utils.h"
#include "../include/jacobi.h"


void stableSelectionSort(matrix* mat, double* a, int n);

int eigengap_heuristic(vector_values_pair pair, int k);

int get_k(double* eigenvalues, int k, int n);

#endif
