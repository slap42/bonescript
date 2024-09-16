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

void bs_scope_store_variable(bs_scope_t* scope, bs_variable_t* var) {
  scope->variables_size++;
  scope->variables = realloc(scope->variables, sizeof(bs_variable_t*) * scope->variables_size);
  scope->variables[scope->variables_size - 1] = var;
}

bs_variable_t* bs_scope_get_variable(bs_scope_t* scope, const char* name, size_t name_length) {
  for (size_t i = 0; i < scope->variables_size; ++i) {
    if (strncmp(name, scope->variables[i]->name, name_length) == 0) {
      return scope->variables[i];
    }
  }
  // TODO: Search next scope
  return NULL;
}
