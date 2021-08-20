#ifndef PROJECT_TEST_UTILS_H
#define PROJECT_TEST_UTILS_H

#define assertf(expr, msg) (!expr ? printf("FAIL:\t%s\n", msg), assert(expr) : assert(!expr))

#define start_test(TEST) printf("Starting Test:\t%s\n", TEST)
#define end_test(TEST) printf("PASS!")

#define true 1
#define false 0

#endif
