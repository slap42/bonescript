#include "bonescript/lexer/lexer.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "bonescript/error/error_internal.h"

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
  char string_delimiter = lexer->ptr[0];
  lexer->ptr++;
  bs_token_t* token = bs_token_create(BS_TOKEN_STRING, lexer->ptr, 0);
  while (lexer->ptr[0] != string_delimiter && (lexer->ptr-1)[0] != '\\') {
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


// Parse either an int or a float
static bs_token_t* bs_lexer_parse_number(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_INT, lexer->ptr, 0);
  bool decimalpointfound = false;
  while (isalnum(lexer->ptr[0]) || lexer->ptr[0] == '.') {
    if (lexer->ptr[0] == '.') {
      decimalpointfound = true;
    }
    lexer->ptr++;
  };
  if (decimalpointfound) {
    token->type = BS_TOKEN_FLOAT;
  }
  token->length = lexer->ptr - token->value;
  return token;
}

// Parse either = or ==
static bs_token_t* bs_lexer_parse_equals(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_EQUAL, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_DOUBLEEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either ! or !=
static bs_token_t* bs_lexer_parse_not(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_NOT, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_NOTEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either < or <=
static bs_token_t* bs_lexer_parse_langlebracket(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_LANGLEBRACKET, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_LESSOREQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either > or >=
static bs_token_t* bs_lexer_parse_ranglebracket(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_RANGLEBRACKET, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_GREATEROREQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either +, ++ or +=
static bs_token_t* bs_lexer_parse_plus(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_PLUS, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '+') {
    token->type = BS_TOKEN_PLUSPLUS;
    token->length = 2;
    lexer->ptr++; 
  }
  else if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_PLUSEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either -, -- or -=
static bs_token_t* bs_lexer_parse_minus(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_MINUS, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '-') {
    token->type = BS_TOKEN_MINUSMINUS;
    token->length = 2;
    lexer->ptr++; 
  }
  else if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_MINUSEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either * or *=
static bs_token_t* bs_lexer_parse_multiply(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_MULTIPLY, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_MULTIPLYEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either / or /=
static bs_token_t* bs_lexer_parse_divide(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_DIVIDE, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_DIVIDEEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

// Parse either % or %=
static bs_token_t* bs_lexer_parse_modulo(bs_lexer_t* lexer) {
  bs_token_t* token = bs_token_create(BS_TOKEN_MODULO, lexer->ptr, 1);
  lexer->ptr++;
  if (lexer->ptr[0] == '=') {
    token->type = BS_TOKEN_MODULOEQUAL;
    token->length = 2;
    lexer->ptr++; 
  }
  return token;
}

bs_token_t* bs_lexer_next_token(bs_lexer_t* lexer) {
  bs_lexer_skip_whitespace(lexer);
  if (lexer->ptr[0] == '\0' || !isascii(lexer->ptr[0])) {
    return NULL;
  }

  // Single character tokens
  switch (lexer->ptr[0]) {
    case '.': lexer->ptr++; return bs_token_create(BS_TOKEN_POINT,       lexer->ptr - 1, 1);
    case ':': lexer->ptr++; return bs_token_create(BS_TOKEN_COLON,       lexer->ptr - 1, 1);
    case ';': lexer->ptr++; return bs_token_create(BS_TOKEN_SEMICOLON,   lexer->ptr - 1, 1);
    case ',': lexer->ptr++; return bs_token_create(BS_TOKEN_COMMA,       lexer->ptr - 1, 1);
    case '^': lexer->ptr++; return bs_token_create(BS_TOKEN_CARET,       lexer->ptr - 1, 1);
    case '(': lexer->ptr++; return bs_token_create(BS_TOKEN_LPAREN,      lexer->ptr - 1, 1);
    case ')': lexer->ptr++; return bs_token_create(BS_TOKEN_RPAREN,      lexer->ptr - 1, 1);
    case '[': lexer->ptr++; return bs_token_create(BS_TOKEN_LSQBRACKET,  lexer->ptr - 1, 1);
    case ']': lexer->ptr++; return bs_token_create(BS_TOKEN_RSQBRACKET,  lexer->ptr - 1, 1);
    case '{': lexer->ptr++; return bs_token_create(BS_TOKEN_LCURLYBRACE, lexer->ptr - 1, 1);
    case '}': lexer->ptr++; return bs_token_create(BS_TOKEN_RCURLYBRACE, lexer->ptr - 1, 1);
  }

  // Multi character tokens
  if (lexer->ptr[0] == '"' || lexer->ptr[0] == '\'') {
    return bs_lexer_parse_string(lexer);
  }
  else if (isalpha(lexer->ptr[0])) {
    return bs_lexer_parse_id(lexer);
  }
  else if (isdigit(lexer->ptr[0])) {
    return bs_lexer_parse_number(lexer);
  }
  else if (lexer->ptr[0] == '=') {
    return bs_lexer_parse_equals(lexer);
  }
  else if (lexer->ptr[0] == '<') {
    return bs_lexer_parse_langlebracket(lexer);
  }
  else if (lexer->ptr[0] == '>') {
    return bs_lexer_parse_ranglebracket(lexer);
  }
  else if (lexer->ptr[0] == '!') {
    return bs_lexer_parse_not(lexer);
  }
  else if (lexer->ptr[0] == '+') {
    return bs_lexer_parse_plus(lexer);
  }
  else if (lexer->ptr[0] == '-') {
    return bs_lexer_parse_minus(lexer);
  }
  else if (lexer->ptr[0] == '*') {
    return bs_lexer_parse_multiply(lexer);
  }
  else if (lexer->ptr[0] == '/') {
    return bs_lexer_parse_divide(lexer);
  }
  else if (lexer->ptr[0] == '%') {
    return bs_lexer_parse_modulo(lexer);
  }
  // Handle unrecognized tokens
  else {
    bs_error_invoke_callback(BS_ERROR_UNRECOGNIZED_TOKEN, "Unrecognized token");
    return NULL;
  }
}
