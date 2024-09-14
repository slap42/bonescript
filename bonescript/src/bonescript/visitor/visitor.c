#include "bonescript/visitor/visitor.h"

#include <stdio.h>
#include "bonescript/ast/parser.h"
#include "bonescript/error/error_internal.h"

static bs_ast_t* bs_visit(bs_ast_t* ast);

static bs_ast_t* bs_visit_compound(bs_ast_t* ast) {
  printf("bs_visit_compound\n");
  for (size_t i = 0; i < ast->compound.size; ++i) {
    bs_visit(ast->compound.value[i]);
  }
  return NULL;
}

static bs_ast_t* bs_visit_variable_definition(bs_ast_t* ast) {
  printf("bs_visit_variable_definition\n");
}

static bs_ast_t* bs_visit_variable(bs_ast_t* ast) {
  printf("bs_visit_variable\n");
}

static bs_ast_t* bs_visit_function_call(bs_ast_t* ast) {
  printf("bs_visit_function_call\n");
}

static bs_ast_t* bs_visit_string_literal(bs_ast_t* ast) {
  printf("bs_visit_string_literal\n");
}

static bs_ast_t* bs_visit(bs_ast_t* ast) {
  switch (ast->type) {
    case BS_AST_COMPOUND:            return bs_visit_compound(ast);
    case BS_AST_VARIABLE_DEFINITION: return bs_visit_variable_definition(ast);
    case BS_AST_VARIABLE:            return bs_visit_variable(ast);
    case BS_AST_FUNCTION_CALL:       return bs_visit_function_call(ast);
    case BS_AST_STRING_LITERAL:      return bs_visit_string_literal(ast);
  }

  bs_error_invoke_callback(BS_ERROR_RUNTIME_ERROR, "Runtime error: Unexpected ast node type encountered");
  return NULL;
}

int bs_run_interpreted(const char* code) {
  bs_ast_t* ast = bs_parse_tokens(code);
  bs_visit(ast);
  bs_ast_destroy(ast);
}
