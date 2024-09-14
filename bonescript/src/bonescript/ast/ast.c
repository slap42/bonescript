#include "bonescript/ast/ast.h"

#include <stdlib.h>

bs_ast_t* bs_ast_create(int type) {
  bs_ast_t* ast = calloc(1, sizeof(bs_ast_t));
  ast->type = type;
  return ast;
}

void bs_ast_destroy(bs_ast_t* ast) {
  switch (ast->type) {

    case BS_AST_COMPOUND: {
      for (size_t i = 0; i < ast->compound.size; ++i) {
        bs_ast_destroy(ast->compound.value[i]);
      }
      free(ast->compound.value);
      break;
    }

    case BS_AST_FUNCTION_CALL: {
      for (size_t i = 0; i < ast->function_call.args_count; ++i) {
        bs_ast_destroy(ast->function_call.args[i]);
      }
      free(ast->function_call.args);
      break;
    }

    case BS_AST_VARIABLE_DEFINITION: {
      bs_ast_destroy(ast->variable_definition.value);
      break;
    }

  }

  free(ast);
}
