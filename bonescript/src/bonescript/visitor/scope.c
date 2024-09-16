#include "bonescript/visitor/scope.h"

#include <stdlib.h>
#include <string.h>
#include "bonescript/error/error.h"

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

bs_variable_t* bs_scope_get_variable_current_scope_only(bs_scope_t* scope, const char* name, size_t name_length) {
  for (size_t i = 0; i < scope->variables_size; ++i) {
    if (strncmp(name, scope->variables[i]->name, name_length) == 0) {
      return scope->variables[i];
    }
  }
  return NULL;
}

bs_variable_t* bs_scope_store_variable(bs_scope_t* scope, bs_variable_t* var) {
  bs_variable_t* old = bs_scope_get_variable_current_scope_only(scope, var->name, strlen(var->name));
  
  if (old) {
    // Overwrite existing variable
    if (var->type != old->type) {
      BS_ERROR("Tried to assign value of type %d to variable %s of type %d", var->type, var->name, old->type);
    }
    free(old->data);
    old->data = var->data;
    old->data_size = var->data_size;
    free(var->name);
    free(var);
    return old;
  }
  else {
    // Store new variable
    scope->variables_size++;
    scope->variables = realloc(scope->variables, sizeof(bs_variable_t*) * scope->variables_size);
    scope->variables[scope->variables_size - 1] = var;
    return var;
  }
}

bs_variable_t* bs_scope_get_variable(bs_scope_t* scope, const char* name, size_t name_length) {
  bs_variable_t* var = bs_scope_get_variable_current_scope_only(scope, name, name_length);
  if (var) {
    return var;
  }
  // TODO: Search next scope
  return NULL;
}
