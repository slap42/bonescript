#ifndef BONESCRIPT_LEXER_H_
#define BONESCRIPT_LEXER_H_

#include "token.h"

typedef struct BS_LEXER_STRUCT {
  const char* code;
  const char* ptr;
} bs_lexer_t;

bs_lexer_t* bs_lexer_create(const char* code);
void bs_lexer_destroy(bs_lexer_t* lexer);
bs_token_t* bs_lexer_next_token(bs_lexer_t* lexer);

#endif
