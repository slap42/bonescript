#include "bonescript/bonescript.h"

#include <stdio.h>
#include <stdlib.h>
#include "lexer/lexer.h"

void bs_execute(const char* code) {
  bs_lexer_t* lexer = bs_lexer_create(code);
  bs_token_t* token;
  while (token = bs_lexer_next_token(lexer)) {
    printf("<%.*s>\n", (int)token->length, token->value);
    bs_token_destroy(token);
  }
  bs_lexer_destroy(lexer);
}
