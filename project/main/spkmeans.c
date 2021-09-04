#include "../include/DDG.h"
#include "../include/eigengap.h"
#include "../include/jacobi.h"
#include "../include/kmeans.h"
#include "../include/LNORM.h"
#include "../include/parse_file.h"
#include "../include/WAM.h"

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

	if (argc != 4) {
		printf("Wrong number of arguments. Correct format is: ./spkeams <goal> <K> <filename>\n");
		exit(EXIT_FAILURE);
	}

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
	} else {
		printf("BAD GOAL: %s\n", goal_str);
		exit(EXIT_FAILURE);
	}

	K = atoi(argv[2]);

	if (K < 0) {
		printf("BAD K: %d; K must be non-negative.\n", K);
		exit(EXIT_FAILURE);
	}

	args.K = K;

	args.filename = argv[3];

	return args;
}

int main(int argc, char* argv[]) {
	parse_cmd(argc, argv);

	return 0;
}
