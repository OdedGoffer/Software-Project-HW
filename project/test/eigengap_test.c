#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/eigengap.h"


#define TEST "Eigengap_Heuristic"

void TEST_Eigengap_Heuristic_sort() {
	double *a, *b;
	int n, i;

    a = { 4.2, 5.1, 3.3, 2.2, 4.2, 1.0 };
    b = { 1.0, 2.2, 3.3, 4.2, 4.2, 5.1 };

    n = a.Length;
    stableSelectionSort(a, n);
    
    for (i=0; i<n; i++) {
    	assertf(a[i] != b[i], "sorting is incorrect");
    }
}

void TEST_Eigengap_Heuristic() {
	double* a;
	int n, i, k;

    a = { 1.0, 2.2, 3.3, 4.2, 4.2, 5.1 };
    n = a.Length;
    
    k = TEST_Eigengap_Heuristic(a, n);

    assertf(k != 1, "k is incorrect");
}

int main() {
	start_test(TEST);

	TEST_Eigengap_Heuristic_sort();
	TEST_Eigengap_Heuristic();

	end_test(TEST);
}
