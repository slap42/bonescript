#ifndef BONESCRIPT_ERROR_H_
#define BONESCRIPT_ERROR_H_

#include <stdio.h>
#include <stdlib.h>

#define BS_ERROR(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

#endif
