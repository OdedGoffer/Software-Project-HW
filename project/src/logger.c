#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>

void invalid_input() {
	printf("Invalid Input!");
	exit(EXIT_FAILURE);
}

void error_occured() {
	printf("An Error Has Occured");
	exit(EXIT_FAILURE);
}
