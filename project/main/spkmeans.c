#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/DDG.h"
#include "../include/eigengap.h"
#include "../include/kmeans.h"
#include "../include/LNORM.h"
#include "../include/parse_file.h"
#include "../include/WAM.h"
#include "../include/logger.h"
#include "../include/calculate_centroids.h"

typedef enum {
	_SPK,
	_WAM,
	_DDG,
	_LNORM,
	_JACOBI
} goal;

typedef struct {
	goal goal;
	int K;
	char* filename;
} args;

void print_eigenvalues(vectors_values_pair vvp) {
	int i;

	for (i = 0; i < vvp.n - 1; i++) {
		printf("%.4f,", vvp.eigenvalues[i]);
	}
	printf("%.4f\n", vvp.eigenvalues[vvp.n - 1]);
}

args parse_cmd(int argc, char* argv[]) {
	args args;
	char* goal_str;
	int K;

	if (argc != 4) log_err("Wrong number of arguments. Correct format is: ./spkeams <goal> <K> <filename>\n");

	goal_str = argv[2];

	if (strcmp(goal_str, "spk") == 0) {
		args.goal = _SPK;
	} else if (strcmp(goal_str, "wam") == 0) {
		args.goal = _WAM;
	} else if (strcmp(goal_str, "ddg") == 0) {
		args.goal = _DDG;
	} else if (strcmp(goal_str, "lnorm") == 0) {
		args.goal = _LNORM;
	} else if (strcmp(goal_str, "jacobi") == 0) {
		args.goal = _JACOBI;
	} else log_err("BAD GOAL: %s\n", goal_str);

	K = atoi(argv[1]);

	if (K < 0) log_err("BAD K: %d; K must be non-negative.\n", K);

	args.K = K;

	args.filename = argv[3];

	return args;
}

void do_spkmeans(matrix* input, int K) {
	matrix *W, *D, *Lnorm, *T, *centroids;
	vectors_values_pair pair;
	vectors_k_pair vectors_pair;
	int* centroids_arr;

	W = WAM(input);

	D = DDG(W);

	Lnorm = LNORM(W, D);


	pair = jacobi(Lnorm);

	vectors_pair = eigengap_heuristic(pair, K);


	T = vectors_pair.vectors;
	K = vectors_pair.k;

	centroids_arr = kmeans(T, K);

	centroids = calculate_centroids(centroids_arr, input, K);

	matrix_free(W);
	matrix_free(D);
	matrix_free(T);
	eigenvectors_free(pair);
	free(centroids_arr);

	matrix_print(centroids);
	matrix_free(centroids);
	matrix_free(input);
}

void do_wam(matrix* input) {
	matrix* res;

	res = WAM(input);
	matrix_print(res);
	matrix_free(res);
	matrix_free(input);
}

void do_ddg(matrix* input) {
	matrix *W, *res;

	W = WAM(input);
	res = DDG(W);
	matrix_print(res);
	matrix_free(W);
	matrix_free(res);
	matrix_free(input);
}

void do_lnorm(matrix* input) {
	matrix *res, *W, *D;

	W = WAM(input);
	D = DDG(W);
	res = LNORM(W, D);

	matrix_print(res);

	matrix_free(W);
	matrix_free(D);
	matrix_free(res);
	matrix_free(input);
}

void do_jacobi(matrix* input) {
	vectors_values_pair pair;
	if (input->n != input->m) log_err(
			"Jacobi matrix input should be symetrical.\n");

	pair = jacobi(input);

	if (!pair.eigenvectors) log_err("Jacobi algorithm did not converge after maximum iterations.");

	print_eigenvalues(pair);
	matrix_print(pair.eigenvectors);
	eigenvectors_free(pair);
}

int main(int argc, char* argv[]) {
	args args;
	matrix* input;

	args = parse_cmd(argc, argv);
	input = read_csv(args.filename);
	if (args.K >= input->m) log_err(
		"BAD K: %d; K must be less than vectors count: %d\n", args.K, input->m);

	switch (args.goal) {
		case _SPK:
			do_spkmeans(input, args.K);
			break;
		case _WAM:
			do_wam(input);
			break;
		case _DDG:
			do_ddg(input);
			break;
		case _LNORM:
			do_lnorm(input);
			break;
		case _JACOBI:
			do_jacobi(input);
			break;
	}

	return 0;
}
