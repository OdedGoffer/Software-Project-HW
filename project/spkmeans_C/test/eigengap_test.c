#include "test_utils/test_utils.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/eigengap.h"
#include "../include/matrix_utils.h"


#define TEST "Eigengap_Heuristic"

void TEST_Eigengap_Heuristic_sort() {
	int i;
    matrix* mat;
    double a[3] = { 4.2, 5.1, 3.3 };
    double b[3] = { 3.3, 4.2, 5.1 };

    mat = matrix_init(3, 3);
    matrix_set(0, 0, mat, 4);
    matrix_set(0, 1, mat, 2);
    matrix_set(0, 2, mat, 2);
    matrix_set(1, 0, mat, 1);
    matrix_set(1, 1, mat, 0);
    matrix_set(1, 2, mat, 2);
    matrix_set(2, 0, mat, 5);
    matrix_set(2, 1, mat, 3);
    matrix_set(2, 2, mat, 2);

    /* mat =
     * [ 4, 2, 2
     *   1, 0, 2 
         5, 3, 2 ]
     */

    stableSelectionSort(mat, a, 3);

    for (i=0; i<3; i++) {
        assertf(a[i] == b[i], "sorting is incorrect");
    }

    assertf(mat->rows[0]->values[1] == 3, "wrong 0,0 value");
    assertf(mat->rows[1]->values[1] == 2, "wrong 0,1 value");
    assertf(mat->rows[2]->values[1] == 0, "wrong 0,1 value");

    matrix_free(mat);
}

void TEST_get_k() {
	int k;

    double a[6] = { 1.0, 2.2, 3.3, 4.2, 4.2, 5.1 };
    
    k = get_k(a, 0, 6);

    assertf(k == 1, "k is incorrect");
}

int main() {
	start_test(TEST);

	TEST_Eigengap_Heuristic_sort();
	TEST_get_k();

	end_test(TEST);
}
