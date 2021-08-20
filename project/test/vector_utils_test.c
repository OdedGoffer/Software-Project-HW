#include "test_utils.h"
#include <stdio.h>
#include <assert.h>

#define TEST "vector_utils"

void TEST_vector_init() {
    assertf(false, "dummy test, should fail");
}

int main() {
    start_test(TEST);

    TEST_vector_init();

    end_test(TEST);
}
