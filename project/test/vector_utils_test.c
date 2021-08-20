#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define clean_errno() (errno == 0 ? "None" : strerror(errno))
#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%d:) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define assertf(A, M, ...) if(!(A)) {log_error(M, ##__VA_ARGS__); assert(A); }
#define true 1
#define false 0

void TEST_vector_init() {
    assertf(false, "dummy test, should fail");
}

int main() {
    TEST_vector_init();
}
