#include "bonescript/lexer/token.h"

#include <stdlib.h>

bs_token_t* bs_token_create(int type, const char* value, unsigned int length) {
  bs_token_t* token = malloc(sizeof(bs_token_t));
  token->type = type;
  token->value = value;
  token->length = length;
  return token;
}

void bs_token_destroy(bs_token_t* token) {
  free(token);
}
