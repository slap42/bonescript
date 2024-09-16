#include "bonescript/visitor/visitor.h"

#include <stdlib.h>
#include <string.h>
#include "bonescript/ast/parser.h"
#include "bonescript/error/error_internal.h"
#include "bonescript/visitor/function.h"
#include "bonescript/visitor/import.h"

static bs_variable_t* bs_visit_compound(bs_ast_t* ast, bs_scope_t* scope) {
  for (size_t i = 0; i < ast->compound.size; ++i) {
    bs_visit(ast->compound.value[i], scope);
  }
  return NULL;
}

static bs_variable_t* bs_visit_variable_definition(bs_ast_t* ast, bs_scope_t* scope) {
  bs_variable_t* var = bs_visit(ast->variable_definition.value, scope);
  bs_variable_assign_name(var, ast->variable_definition.name, ast->variable_definition.name_length);
  bs_scope_store_variable(scope, var);
  return var;
}

static bs_variable_t* bs_visit_variable(bs_ast_t* ast, bs_scope_t* scope) {
  bs_variable_t* var = bs_scope_get_variable(scope, ast->variable.name, ast->variable.name_length);
  if (!var) {
    bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "Undefined variable");
  }
  return var;
}

static bs_variable_t* bs_visit_function_call(bs_ast_t* ast, bs_scope_t* scope) {
  // The only "builtin" function is import - this is used to define all other functions as first class objects
  if (strncmp(ast->function_call.name, "import", ast->function_call.name_length) == 0) {
    if (ast->function_call.args_count != 1) {
      bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "import takes exactly one argument");
    }
    bs_variable_t* var = bs_visit(ast->function_call.args[0], scope);
    if (var->type != BS_VARIABLE_TYPE_STRING) {
      bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "import requires a string argument");
    }
    bs_import_symbols(scope, var);
    bs_variable_destroy(var);
    return NULL;
  }
  // If it's not import, look it up in the variables table and call it
  else {
    bs_variable_t* func = bs_scope_get_variable(scope, ast->function_call.name, ast->function_call.name_length);
    if (!func) {
      bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "Undefined function");
    }
    else if (func->type != BS_VARIABLE_TYPE_FUNCTION) {
      bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "Tried to call a variable that's not a function");
    }
    bs_function_ptr_t f = (bs_function_ptr_t)func->data;
    return f(ast->function_call.args, ast->function_call.args_count, scope);
  }
}

static bs_variable_t* bs_visit_string_literal(bs_ast_t* ast, bs_scope_t* scope) {
  bs_variable_t* string = bs_variable_create(BS_VARIABLE_TYPE_STRING);
  bs_variable_assign_data(string, ast->string_literal.value, ast->string_literal.value_length);
  return string;
}

bs_variable_t* bs_visit(bs_ast_t* ast, bs_scope_t* scope) {
  switch (ast->type) {
    case BS_AST_COMPOUND:            return bs_visit_compound(ast, scope);
    case BS_AST_VARIABLE_DEFINITION: return bs_visit_variable_definition(ast, scope);
    case BS_AST_VARIABLE:            return bs_visit_variable(ast, scope);
    case BS_AST_FUNCTION_CALL:       return bs_visit_function_call(ast, scope);
    case BS_AST_STRING_LITERAL:      return bs_visit_string_literal(ast, scope);
  }

  bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "Runtime error: Unexpected ast node type encountered");
  return NULL;
}
