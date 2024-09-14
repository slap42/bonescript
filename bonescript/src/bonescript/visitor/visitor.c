#include "bonescript/visitor/visitor.h"

#include <stdio.h>
#include "bonescript/ast/parser.h"
#include "bonescript/visitor/scope.h"
#include "bonescript/error/error_internal.h"

static bs_ast_t* bs_visit(bs_ast_t* ast, bs_scope_t* scope);

static bs_ast_t* bs_visit_compound(bs_ast_t* ast, bs_scope_t* scope) {
  printf("bs_visit_compound\n");
  for (size_t i = 0; i < ast->compound.size; ++i) {
    bs_visit(ast->compound.value[i], scope);
  }
  return NULL;
}

static bs_ast_t* bs_visit_variable_definition(bs_ast_t* ast, bs_scope_t* scope) {
  printf("bs_visit_variable_definition\n");
}

static bs_ast_t* bs_visit_variable(bs_ast_t* ast, bs_scope_t* scope) {
  printf("bs_visit_variable\n");
}

static bs_ast_t* bs_visit_function_call(bs_ast_t* ast, bs_scope_t* scope) {
  printf("bs_visit_function_call\n");
}

static bs_ast_t* bs_visit_string_literal(bs_ast_t* ast, bs_scope_t* scope) {
  printf("bs_visit_string_literal\n");
}

static bs_ast_t* bs_visit(bs_ast_t* ast, bs_scope_t* scope) {
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

int bs_run_interpreted(const char* code) {
  bs_ast_t* ast = bs_parse_tokens(code);
  bs_scope_t* scope = bs_scope_create();
  bs_visit(ast, scope);
  bs_scope_destroy(scope);
  bs_ast_destroy(ast);
}
