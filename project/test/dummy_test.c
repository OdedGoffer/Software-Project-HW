#include "test_utils/test_utils.h"
#include <stdio.h>
#include <assert.h>
#include "../include/dummy.h"

#define TEST "dummy_test"

void TEST_dummy() {
	int i;

	i = dummy_func();
	assertf(i == 1, "dummy failure messsage");
}

int main() {
	start_test(TEST);

	TEST_dummy();

	end_test(TEST);
}
