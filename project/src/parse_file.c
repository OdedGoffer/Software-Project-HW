#include "../include/parse_file.h"
#include <stdio.h>
#include <stdlib.h>
#include "../include/logger.h"

/*
 *
 * PARSE FILE
 *
 */

matrix* read_csv(char* filename) {
	FILE* fp;
	int result, i = 0, n = 0;
	double val, values[256];
	char c;
	vector *v=NULL, *v_new;
	matrix* res;

	fp = fopen(filename, "r");
	if (!fp) error_occured();

	while(fscanf(fp, "%lf%c", &val, &c) == 2) {
		if (c != ',') break;
		n++;
	}
	n++;

	res = matrix_init(n, 0);
	rewind(fp);

	while (fscanf(fp, "%lf", &val) == 1) {
		values[i] = val;
		i++;
		if (i == n) {
			v = vector_init(values, n);
			v_new = vector_copy(v);
			matrix_add_row(res, v_new);
			i = 0;
		}
		if (!fscanf(fp, "%c", &c)) {
			break;
		}
	}

	result = fclose(fp);
	if (result == EOF) error_occured();
	if (v) free(v);

	return res;
}
