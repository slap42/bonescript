#ifndef BONESCRIPT_ERROR_H_
#define BONESCRIPT_ERROR_H_

#include <stdio.h>
#include <signal.h>

#define BS_ERROR(...) { fprintf(stderr, __VA_ARGS__); raise(SIGTRAP); }

#endif
