#ifndef BONESCRIPT_TOKEN_H_
#define BONESCRIPT_TOKEN_H_

typedef struct BS_TOKEN_STRUCT {

  enum {
    BS_TOKEN_ID,
    BS_TOKEN_EQUALS,
    BS_TOKEN_STRING,
    BS_TOKEN_SEMICOLON,
    BS_TOKEN_LPAREN,
    BS_TOKEN_RPAREN,
    BS_TOKEN_INT,
    BS_TOKEN_FLOAT,
  } type;

  const char* value;
  size_t length;

} bs_token_t;

bs_token_t* bs_token_create(int type, const char* value, unsigned int length);
void bs_token_destroy(bs_token_t* token);

#endif
