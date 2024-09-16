#include <bonescript/bonescript.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <bonescript/ast/parser.h>
#include <bonescript/error.h>
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

static void error_callback(bs_error_t error_code, const char* error_string) {
  fprintf(stderr, "Bonescript error: %s\n", error_string);
  exit(1);
}

int main() {
  bs_error_set_callback(error_callback);
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
