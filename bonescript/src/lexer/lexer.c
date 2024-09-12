#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>

bs_lexer_t* bs_lexer_create(const char* code) {
  bs_lexer_t* lexer = malloc(sizeof(bs_lexer_t));
  lexer->code = code;
  lexer->ptr = code;
  return lexer;
}

void bs_lexer_destroy(bs_lexer_t* lexer) {
  free(lexer);
}

static void bs_lexer_skip_whitespace(bs_lexer_t* lexer) {
  while (isspace(lexer->ptr[0])) {
    lexer->ptr++;
  }
}

static bs_token_t* bs_lexer_parse_string(bs_lexer_t* lexer) {
  lexer->ptr++;
  bs_token_t* token = bs_token_create(BS_TOKEN_STRING, lexer->ptr, 0);
  // TODO: allow \" in strings
  while (lexer->ptr[0] != '"') {
    lexer->ptr++;
  };
  token->length = lexer->ptr - token->value;
  lexer->ptr++;
  return token;
}

static bs_token_t* bs_lexer_parse_id(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_ID, lexer->ptr, 0);
  while (isalnum(lexer->ptr[0])) {
    lexer->ptr++;
  };
  token->length = lexer->ptr - token->value;
  return token;
}

static bs_token_t* bs_lexer_parse_int(bs_lexer_t* lexer) {
  // TODO: Allow parsing hex, binary, etc.
  // TODO: Allow parsing floats
  bs_token_t* token = bs_token_create(BS_TOKEN_INT, lexer->ptr, 0);
  while (isdigit(lexer->ptr[0])) {
    lexer->ptr++;
  };
  token->length = lexer->ptr - token->value;
  return token;
}

bs_token_t* bs_lexer_next_token(bs_lexer_t* lexer) {
  bs_lexer_skip_whitespace(lexer);
  
  if (lexer->ptr[0] == '\0') {
    return NULL;
  }
  
  bs_token_t* token;

  // Single character tokens
  switch (lexer->ptr[0]) {
    case '=': token = bs_token_create(BS_TOKEN_EQUALS,    lexer->ptr, 1); lexer->ptr++; return token;
    case ';': token = bs_token_create(BS_TOKEN_SEMICOLON, lexer->ptr, 1); lexer->ptr++; return token;
    case '(': token = bs_token_create(BS_TOKEN_LPAREN,    lexer->ptr, 1); lexer->ptr++; return token;
    case ')': token = bs_token_create(BS_TOKEN_RPAREN,    lexer->ptr, 1); lexer->ptr++; return token;
  }

  // Multi character tokens
  if (lexer->ptr[0] == '"') {
    token = bs_lexer_parse_string(lexer);
  }
  else if (isalpha(lexer->ptr[0])) {
    token = bs_lexer_parse_id(lexer);
  }
  else if (isdigit(lexer->ptr[0])) {
    token = bs_lexer_parse_int(lexer);
  }

  return token;
}
