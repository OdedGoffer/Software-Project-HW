#include "../include/DDG.h"
#include "../include/eigengap.h"
#include "../include/jacobi.h"
#include "../include/kmeans.h"
#include "../include/LNORM.h"
#include "../include/parse_file.h"
#include "../include/WAM.h"
#include "../include/logger.h"

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

}

void do_wam(matrix* input) {

}

void do_ddg(matrix* input) {

}

void do_lnorm(matrix* input) {

}

void do_jacobi(matrix* input) {

}

int main(int argc, char* argv[]) {
	args args;
	matrix* input;

	args = parse_cmd(argc, argv);

	input = read_csv(args.filename);

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
