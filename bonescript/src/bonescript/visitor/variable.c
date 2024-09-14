#include "bonescript/visitor/variable.h"

#include <stdlib.h>
#include <string.h>

bs_variable_t* bs_variable_string_create(const char* name, size_t name_length, const char* str, size_t str_length) {
  bs_variable_t* variable = malloc(sizeof(bs_variable_t));
  variable->name = malloc(name_length * sizeof(char));
  strncpy(variable->name, name, name_length);
  variable->string.str = malloc(str_length * sizeof(char));
  strncpy(variable->string.str, str, str_length);
  return variable;
}

void bs_variable_destroy(bs_variable_t* variable) {
  switch (variable->type) {
    case BS_VARIABLE_STRING: {
      free(variable->string.str);
    }
  }
  
  free(variable->name);
  free(variable);
}
