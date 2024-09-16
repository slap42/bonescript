#ifndef BONESCRIPT_ERROR_INTERNAL_H_
#define BONESCRIPT_ERROR_INTERNAL_H_

#include "bonescript/error.h"

void bs_error_invoke_callback(bs_error_t error_code, const char* error_string);

#endif
