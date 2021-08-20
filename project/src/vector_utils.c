#include <stdlib.h>
#include <assert.h>
#include "../include/vector_utils.h"

vector *vector_init(double *vals, int N) {
   vector *v;

    v = (vector *) malloc(sizeof(vector));
    assert(v != NULL);
    v->vals = vals;
    v->N = N;
    return v;
}
