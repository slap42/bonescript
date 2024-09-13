#include "bonescript/ast/parser.h"

#include <stdlib.h>
#include <string.h>
#include "bonescript/error/error_internal.h"
#include "bonescript/lexer/lexer.h"

typedef struct BS_PARSER_STRUCT {
  bs_lexer_t* lexer;
  bs_token_t* current_token;
} bs_parser_t;

static void bs_parser_eat(bs_parser_t* parser, enum BS_TOKEN_TYPE type) {
  if (parser->current_token->type == type) {
    parser->current_token = bs_lexer_next_token(parser->lexer);
  }
  else {
    bs_error_invoke_callback(BS_ERROR_UNEXPECTED_TOKEN, "Unexpected token");
  }
}

static bs_ast_t* bs_parse_string(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_STRING);
  ast->string.value = parser->current_token->value;
  ast->string.value_length = parser->current_token->length;
  bs_parser_eat(parser, BS_TOKEN_STRING);
  return ast;
}

static bs_ast_t* bs_parse_expression(bs_parser_t* parser) {
  switch (parser->current_token->type) {
    case BS_TOKEN_STRING: return bs_parse_string(parser);
    default: {
      bs_error_invoke_callback(BS_ERROR_UNEXPECTED_TOKEN, "Unrecognized expression");
      return NULL;
    }
  }
}

static bs_ast_t* bs_parse_string_variable_definition(bs_parser_t* parser) {
  bs_parser_eat(parser, BS_TOKEN_ID); // string
  bs_ast_t* ast = bs_ast_create(BS_AST_STRING_VARIABLE_DEFINITION);
  ast->string_variable_definition.name = parser->current_token->value;
  ast->string_variable_definition.name_length = parser->current_token->length;
  bs_parser_eat(parser, BS_TOKEN_ID); // name
  bs_parser_eat(parser, BS_TOKEN_EQUAL); // =
  ast->string_variable_definition.value = bs_parse_expression(parser);
  return ast;
}

static bs_ast_t* bs_parse_function_call(bs_parser_t* parser) {
  return NULL;
}

static bs_ast_t* bs_parse_variable(bs_parser_t* parser) {
  const char* token_value = parser->current_token->value;
  size_t token_length = parser->current_token->length;

  bs_parser_eat(parser, BS_TOKEN_ID); // var name or function name

  // If we encounter a lparen, assume this is a function call
  if (parser->current_token->type == BS_TOKEN_LPAREN) {
    return bs_parse_function_call(parser);
  }

  bs_ast_t* ast = bs_ast_create(BS_AST_VARIABLE);
  ast->variable.name = token_value;
  ast->variable.name_length = token_length;
  return ast;
}

static bs_ast_t* bs_parse_id(bs_parser_t* parser) {  
  if (strncmp(parser->current_token->value, "string", parser->current_token->length) == 0) {
    return bs_parse_string_variable_definition(parser);
  }
  else {
    return bs_parse_variable(parser);
  }
}

static bs_ast_t* bs_parse_statement(bs_parser_t* parser) {
  switch (parser->current_token->type) {
    case BS_TOKEN_ID: return bs_parse_id(parser);
    default: {
      bs_error_invoke_callback(BS_ERROR_UNEXPECTED_TOKEN, "Invalid statement");
      return NULL;
    }
  }
}

static bs_ast_t* bs_parse_compound(bs_parser_t* parser) {
  bs_ast_t* compound = bs_ast_create(BS_AST_COMPOUND);

  compound->compound.size = 1;
  compound->compound.value = malloc(sizeof(bs_ast_t*));
  compound->compound.value[0] = bs_parse_statement(parser);

  while (parser->current_token->type == BS_TOKEN_SEMICOLON) {
    bs_parser_eat(parser, BS_TOKEN_SEMICOLON);

    compound->compound.size++;
    compound->compound.value = realloc(compound->compound.value, compound->compound.size * sizeof(bs_ast_t*));
    compound->compound.value[compound->compound.size - 1] = bs_parse_statement(parser);
  }

  return compound;
}

bs_ast_t* bs_parse_tokens(const char* code) {
  bs_parser_t* parser = malloc(sizeof(bs_parser_t));
  parser->lexer = bs_lexer_create(code);
  parser->current_token = bs_lexer_next_token(parser->lexer);
  bs_ast_t* ast = bs_parse_compound(parser);
  bs_lexer_destroy(parser->lexer);
  free(parser);
  return ast;
}
