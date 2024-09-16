#include "bonescript/visitor/variable.h"

#include <stdlib.h>
#include <string.h>

bs_variable_t* bs_variable_create(int type) {
  bs_variable_t* var = calloc(1, sizeof(bs_variable_t));
  var->type = type;
  return var;
}

void bs_variable_assign_name(bs_variable_t* var, const char* name, size_t name_length) {
  var->name = realloc(var->name, name_length + 1);
  memset(var->name, 0, name_length + 1);
  strncpy(var->name, name, name_length);
}

void bs_variable_assign_data(bs_variable_t* var, const void* data, size_t data_size) {
  var->data_size = data_size;
  var->data = realloc(var->data, data_size + 1);
  memset(var->data, 0, data_size + 1);
  memcpy(var->data, data, data_size);
}

void bs_variable_destroy(bs_variable_t* var) {
  if (var->name) {
    free(var->name);
  }
  if (var->type != BS_VARIABLE_TYPE_BUILTIN_FUNCTION && var->data) {
    free(var->data);
  }
  free(var);
}
