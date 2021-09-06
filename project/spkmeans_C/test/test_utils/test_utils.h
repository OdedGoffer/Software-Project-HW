#ifndef PROJECT_TEST_UTILS_H
#define PROJECT_TEST_UTILS_H

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/vector_utils.h"
#include "../../include/matrix_utils.h"

#define assertf(expr, msg) (!(expr) ? printf("FAIL:\t%s\n", msg), assert(expr) : assert(expr))
#define start_test(TEST) printf("Starting Test:\t%s\n", TEST)
#define end_test(TEST) printf("PASS!\n"); return 0;
#define true 1
#define false 0

#endif
