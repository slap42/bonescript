#include <bonescript/bonescript.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <bonescript/ast/parser.h>
#include <bonescript/lexer/lexer.h>

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

static void test_lexer() {
  char* code = load_file("code/test-lexer.bs");

  bs_lexer_t* lexer = bs_lexer_create(code);
  bs_token_t* token;
  while (token = bs_lexer_next_token(lexer)) {
    // printf("<%.*s>\n", (int)token->length, token->value);
    bs_token_destroy(token);
  }
  bs_lexer_destroy(lexer);

  free(code);
}

static void test_parser() {
  char* code = load_file("code/test-parser.bs");
  bs_ast_t* ast = bs_parse_tokens(code);
  bs_ast_print(ast);
  free(code);
}

static void error_callback(bs_error_t error_code, const char* error_string) {
  fprintf(stderr, "Bonescript error: %s\n", error_string);
  exit(1);
}

void main() {
  bs_error_set_callback(error_callback);
  test_lexer();
  test_parser();
}
