#ifndef BONESCRIPT_H_
#define BONESCRIPT_H_

#include <stdbool.h>

typedef (*bs_error_callback)(int error_code, const char* error_string);

void bs_execute(const char* code);
bool bs_set_error_callback(bs_error_callback error_callback);

#endif
