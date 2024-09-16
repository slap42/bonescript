#include "bonescript/error.h"

static void bs_default_error_callback(bs_error_t error_code, const char* error_string) {
  // Do nothing
}

static bs_error_callback bs_error_global_callback = bs_default_error_callback;

void bs_error_set_callback(bs_error_callback error_callback) {
  bs_error_global_callback = error_callback;
}

void bs_error_invoke_callback(bs_error_t error_code, const char* error_string) {
  bs_error_global_callback(error_code, error_string);
}
