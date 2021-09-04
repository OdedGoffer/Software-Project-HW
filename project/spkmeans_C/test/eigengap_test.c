#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/eigengap.h"


#define TEST "Eigengap_Heuristic"

void TEST_Eigengap_Heuristic_sort() {
	int i;
    double a[6] = { 4.2, 5.1, 3.3, 2.2, 4.2, 1.0 };
    double b[6] = { 1.0, 2.2, 3.3, 4.2, 4.2, 5.1 };

    stableSelectionSort(a, 6);
    
    for (i=0; i<6; i++) {
    	assertf(a[i] == b[i], "sorting is incorrect");
    }
}

void TEST_Eigengap_Heuristic() {
	int k;

    double a[6] = { 1.0, 2.2, 3.3, 4.2, 4.2, 5.1 };
    
    k = eigengap_heuristic(a, 6);

    assertf(k == 1, "k is incorrect");
}

int main() {
	start_test(TEST);

	TEST_Eigengap_Heuristic_sort();
	TEST_Eigengap_Heuristic();

	end_test(TEST);
}
