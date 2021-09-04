#ifndef PROJECT_PARSE_FILE_H
#define PROJECT_PARSE_FILE_H

#include "matrix_utils.h"
#include <stdio.h>
#include <assert.h>

/*Parse file at path 'filename' to matrix'*/
matrix* read_csv(char* filename);

#endif
