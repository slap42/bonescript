#ifndef BONESCRIPT_ERROR_H_
#define BONESCRIPT_ERROR_H_

typedef enum {
  BS_ERROR_UNRECOGNIZED_TOKEN,
  BS_ERROR_UNEXPECTED_TOKEN,
} bs_error_t;

typedef void(*bs_error_callback)(bs_error_t error_code, const char* error_string);

void bs_error_set_callback(bs_error_callback error_callback);

#endif
