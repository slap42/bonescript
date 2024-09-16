#include <bonescript/bonescript.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <bonescript/ast/parser.h>
#include <bonescript/lexer/lexer.h>
#include <bonescript/visitor/visitor.h>
#include "utils/print_ast.h"

static char* load_file(const char* path) {
  FILE* file = fopen(path, "rb");
  fseek(file, 0L, SEEK_END);
  size_t size = ftell(file);
  char* buf = malloc(size + 1);
  rewind(file);
  fread(buf, 1, size, file);
  fclose(file);
  buf[size] = '\0';
  return buf;
}

int main() {
  char* code = load_file("bonescript-tests/code/print.bs");
  bs_ast_t* ast = bs_parse_tokens(code);
  bs_ast_print(ast);
  bs_scope_t* scope = bs_scope_create();
  bs_visit(ast, scope);
  bs_scope_destroy(scope);
  bs_ast_destroy(ast);
  free(code);
  return 0;
}
