#include "../include/logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void log_err(char* msg, ...) {
	va_list args;

	va_start(args, msg);
	vfprintf(stderr, msg, args);
	exit(EXIT_FAILURE);
}

void log_info(char* msg, ...) {
	va_list args;

	va_start(args, msg);
	vfprintf(stderr, msg, args);
}
