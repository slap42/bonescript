#ifndef BONESCRIPT_SCOPE_H_
#define BONESCRIPT_SCOPE_H_

#include <stddef.h>
#include "bonescript/visitor/variable.h"

typedef struct BS_SCOPE_STRUCT {
  bs_variable_t** variables;
  size_t variables_size;
  // struct BS_SCOPE_STRUCT* next;
} bs_scope_t;

bs_scope_t* bs_scope_create();
void bs_scope_destroy(bs_scope_t* scope);
bs_variable_t bs_scope_get_variable(bs_scope_t* scope, const char* name);

#endif
