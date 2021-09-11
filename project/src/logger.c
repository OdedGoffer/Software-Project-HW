#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *
 * LOGGER
 *
 */

void invalid_input() {
	printf("Invalid Input!\n");
	exit(EXIT_FAILURE);
}

void error_occured() {
	printf("An Error Has Occured\n");
	exit(EXIT_FAILURE);
}
