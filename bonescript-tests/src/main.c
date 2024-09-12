#include <bonescript/bonescript.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <lexer/lexer.h>

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
    printf("<%.*s>\n", (int)token->length, token->value);
    bs_token_destroy(token);
  }
  bs_lexer_destroy(lexer);

  free(code);
}

static void error_callback(int error_code, const char* error_string) {

}

void main() {
  bs_set_error_callback(error_callback);
  test_lexer();
}
