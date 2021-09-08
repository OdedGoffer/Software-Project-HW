#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/DDG.h"
#include "../include/eigengap.h"
#include "../include/jacobi.h"
#include "../include/kmeans.h"
#include "../include/LNORM.h"
#include "../include/parse_file.h"
#include "../include/WAM.h"
#include "../include/logger.h"
#include "../include/calculate_centroids.h"
#include <string.h>
#include <stdlib.h>

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

args parse_cmd(int argc, char* argv[]) {
	args args;
	char* goal_str;
	int K;

	if (argc != 4) log_err("Wrong number of arguments. Correct format is: ./spkeams <goal> <K> <filename>\n");

	goal_str = argv[1];

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

	K = atoi(argv[2]);

	if (K < 0) log_err("BAD K: %d; K must be non-negative.\n", K);

	args.K = K;

	args.filename = argv[3];

	return args;
}

void do_spkmeans(matrix* input, int K) {
	matrix *W, *D, *Lnorm, *T, *centroids;
	vectors_values_pair pair;
	vectors_k_pair verctors_pair;
	int* centroids_arr;
	int new_k;

	W = WAM(input);
	D = DDG(W);
	Lnorm = LNORM(W, D);
	pair = jacobi(Lnorm);
	verctors_pair = eigengap_heuristic(pair, K);
	T = verctors_pair.vectors;
	new_k = verctors_pair.k;
	centroids_arr = kmeans(T, new_k);
	centroids = calculate_centroids(centroids_arr, input, new_k);
	matrix_print(centroids);

	matrix_free(W);
	matrix_free(D);
	matrix_free(pair.eigenvectors);
	matrix_free(T);
	matrix_free(centroids);
	free(pair.eigenvalues);
	free(centroids_arr);
}

void do_wam(matrix* input) {
	matrix* res;

	res = WAM(input);
	matrix_print(res);
	matrix_free(res);
}

void do_ddg(matrix* input) {
	matrix *W, *res;

	W = WAM(input);
	res = DDG(W);
	matrix_print(res);
	matrix_free(W);
	matrix_free(res);
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
}

void do_jacobi(matrix* input) {
	vectors_values_pair pair;
	if (input->n != input->m) log_err(
			"Jacobi matrix input should be symetrical.\n"); //TODO: make this better.

	pair = jacobi(input);

	if (!pair.eigenvectors) {
		printf("Jacobi algorithm did not converge after maximum iterations.");
		return;
	}

	matrix_print(pair.eigenvectors);
	eigenvectors_free(pair);
}

int main(int argc, char* argv[]) {
	args args;
	matrix* input;

	args = parse_cmd(argc, argv);
	input = read_csv(args.filename);
	if (args.K >= input->m) log_err(
		"BAD K: %d; K must be less than vectors count.\n", args.K);

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

	matrix_free(input);
	return 0;
}
