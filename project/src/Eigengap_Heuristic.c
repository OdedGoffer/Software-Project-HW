#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void stableSelectionSort(double* a, int n) {
	int i, j, min; 
	double key;

	for (i = 0; i < n - 1; i++) {
        min = i;

        for (j = i + 1; j < n; j++) {

        	if (a[min] > a[j]) {
        		min = j;
        	}
 
            key = a[min];

            while (min > i) {
            	a[min] = a[min - 1];
                min--;
            }
            a[i] = key;
        }
    }
}

int Eigengap_Heuristic(double* Eigenvalues, int n) {
	double delta;
	int i, index;
	double max_diff = 0;

	stableSelectionSort(Eigenvalues, n);

	for (i=0; i<n-1; i++) {
		delta = Eigenvalues[i+1] - Eigenvalues[i];
		if (delta > max_diff) {
			max_diff = delta;
			index = i+1;
		}
	}

	return index;
}