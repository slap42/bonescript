#include "bonescript/ast/parser.h"

#include <stdlib.h>
#include <string.h>
#include "bonescript/error/error.h"
#include "bonescript/lexer/lexer.h"

typedef struct BS_PARSER_STRUCT {
  bs_lexer_t* lexer;
  bs_token_t* current_token;
  bs_token_t* previous_token;
} bs_parser_t;

static void bs_parser_eat(bs_parser_t* parser, enum BS_TOKEN_TYPE type) {
  if (parser->current_token->type == type) {
    parser->previous_token = parser->current_token;
    parser->current_token = bs_lexer_next_token(parser->lexer);
  }
  else {
    BS_ERROR("Unexpected token: %.*s\n", (int)parser->current_token->length, parser->current_token->value);
  }
}

static bs_ast_t* bs_parse_id(bs_parser_t* parser);

static bs_ast_t* bs_parse_string_literal(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_STRING_LITERAL);
  ast->string_literal.value = parser->current_token->value;
  ast->string_literal.value_length = parser->current_token->length;
  bs_parser_eat(parser, BS_TOKEN_STRING);
  return ast;
}

static bs_ast_t* bs_parse_int_literal(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_INT_LITERAL);
  ast->int_literal.value = atoi(parser->current_token->value);
  bs_parser_eat(parser, BS_TOKEN_INT);
  return ast;
}

static bs_ast_t* bs_parse_expression(bs_parser_t* parser) {
  switch (parser->current_token->type) {
    case BS_TOKEN_ID:     return bs_parse_id(parser);
    case BS_TOKEN_INT:    return bs_parse_int_literal(parser);
    case BS_TOKEN_STRING: return bs_parse_string_literal(parser);
    default: {
      BS_ERROR("Unrecognized expression %.*s", (int)parser->current_token->length, parser->current_token->value);
      return NULL;
    }
  }
}

static bs_ast_t* bs_parse_variable_definition(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_VARIABLE_DEFINITION);
  ast->variable_definition.type = parser->current_token->value;
  ast->variable_definition.type_length = parser->current_token->length;
  bs_parser_eat(parser, BS_TOKEN_ID); // string, int, class name, etc
  ast->variable_definition.name = parser->current_token->value;
  ast->variable_definition.name_length = parser->current_token->length;
  bs_parser_eat(parser, BS_TOKEN_ID); // name
  bs_parser_eat(parser, BS_TOKEN_EQUAL); // =
  ast->variable_definition.value = bs_parse_expression(parser);
  return ast;
}

static bs_ast_t* bs_parse_function_call(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_FUNCTION_CALL);
  ast->function_call.name = parser->previous_token->value;
  ast->function_call.name_length = parser->previous_token->length;
  ast->function_call.args = NULL;
  ast->function_call.args_count = 0;
  
  bs_parser_eat(parser, BS_TOKEN_LPAREN);
  
  // Handle the case where the function has no args
  if (parser->current_token->type == BS_TOKEN_RPAREN) {
    bs_parser_eat(parser, BS_TOKEN_RPAREN);
    return ast;
  }
  
  // Parse args
  do {
    if (parser->current_token->type == BS_TOKEN_COMMA) {
      bs_parser_eat(parser, BS_TOKEN_COMMA);
    }
    ast->function_call.args_count++;
    ast->function_call.args = realloc(ast->function_call.args, sizeof(bs_ast_t*) * ast->function_call.args_count);
    ast->function_call.args[ast->function_call.args_count - 1] = bs_parse_expression(parser);
  } while (parser->current_token->type == BS_TOKEN_COMMA);
  bs_parser_eat(parser, BS_TOKEN_RPAREN);
  
  return ast;
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
  if (strncmp(parser->current_token->value, "string", parser->current_token->length) == 0 ||
      strncmp(parser->current_token->value, "int", parser->current_token->length) == 0) {
    return bs_parse_variable_definition(parser);
  }
  else {
    return bs_parse_variable(parser);
  }
}

static bs_ast_t* bs_parse_compound(bs_parser_t* parser) {
  bs_ast_t* ast = bs_ast_create(BS_AST_COMPOUND);
  ast->compound.size = 0;
  ast->compound.value = NULL;
  
  do {
    ast->compound.size++;
    ast->compound.value = realloc(ast->compound.value, sizeof(bs_ast_t*) * ast->compound.size);
    ast->compound.value[ast->compound.size - 1] = bs_parse_expression(parser);
    bs_parser_eat(parser, BS_TOKEN_SEMICOLON);
  } while (parser->current_token && parser->current_token->type != BS_TOKEN_SEMICOLON);

  return ast;
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
