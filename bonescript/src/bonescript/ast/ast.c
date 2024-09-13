#include "bonescript/ast/ast.h"

#include <stdio.h>
#include <stdlib.h>

bs_ast_t* bs_ast_create(int type) {
  bs_ast_t* ast = calloc(1, sizeof(bs_ast_t));
  ast->type = type;
  return ast;
}

void bs_ast_destroy(bs_ast_t* ast) {
  free(ast);
}

void bs_ast_print(bs_ast_t* ast) {
  printf("ast:\n");
}
