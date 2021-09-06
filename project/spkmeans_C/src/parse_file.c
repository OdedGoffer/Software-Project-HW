#include "../include/parse_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/logger.h"

matrix* read_csv(char* filename) {
	FILE* fp;
	int result, i = 0, n = 0;
	double val, values[256];
	char c;
	vector *v, *v_new;
	matrix* res;

	fp = fopen(filename, "r");
	if (!fp) log_err("Could not open file: %s\n", filename);

	while(fscanf(fp, "%lf%c", &val, &c) == 2) {
		n++;
		if (c == '\n') break;
	}

	res = matrix_init(n, 0);
	rewind(fp);

	while (fscanf(fp, "%lf%c", &val, &c) == 2) {
		if (c == ',') {
			values[i] = val;
			i++;
		} else if (c == '\n') {
			values[i] = val;
			v = vector_init(values, n);
			v_new = vector_copy(v);
			matrix_add_row(res, v_new);
			i = 0;
		}
	}

	result = fclose(fp);
	assert(result != EOF);
	free(v);

	return res;
}
