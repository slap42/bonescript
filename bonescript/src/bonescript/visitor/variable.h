#ifndef BONESCRIPT_VARIABLE_H_
#define BONESCRIPT_VARIABLE_H_

#include <stddef.h>
#include "bonescript/ast/ast.h"

typedef struct BS_VARIABLE_STRUCT {
  
  enum BS_VARIABLE_TYPE_ENUM {
    BS_VARIABLE_TYPE_UNDEFINED,
    BS_VARIABLE_TYPE_STRING,
    BS_VARIABLE_TYPE_INT,
    BS_VARIABLE_TYPE_BUILTIN_FUNCTION,
  } type;
  
  char* name;
  void* data;
  size_t data_size;
  
} bs_variable_t;

bs_variable_t* bs_variable_create(int type);
void bs_variable_assign_name(bs_variable_t* var, const char* name, size_t name_length);
void bs_variable_assign_data(bs_variable_t* var, const void* data, size_t data_size);
void bs_variable_destroy(bs_variable_t* variable);

#endif
