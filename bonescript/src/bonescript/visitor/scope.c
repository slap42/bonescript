#include "bonescript/visitor/scope.h"

#include <stdlib.h>
#include <string.h>

bs_scope_t* bs_scope_create() {
  bs_scope_t* scope = malloc(sizeof(bs_scope_t));
  scope->variables_size = 0;
  scope->variables = NULL;
  return scope;
}

void bs_scope_destroy(bs_scope_t* scope) {
  // TODO: Free next scope
  for (size_t i = 0; i < scope->variables_size; ++i) {
    bs_variable_destroy(scope->variables[i]);
  }
  free(scope);
}

bs_variable_t bs_scope_get_variable(bs_scope_t* scope, const char* name) {
  for (size_t i = 0; i < scope->variables_size; ++i) {
    if (strcmp(name, scope->variables[i]->name) == 0) {
      
    }
  }
  // TODO: Search next scope
}
