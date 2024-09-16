#include "bonescript/bonescript.h"

#include "bonescript/ast/parser.h"
#include "bonescript/lexer/lexer.h"
#include "bonescript/visitor/visitor.h"

int bs_execute(const char* code) {
  bs_ast_t* ast = bs_parse_tokens(code);
  bs_scope_t* scope = bs_scope_create();
  bs_variable_t* result = bs_visit(ast, scope);
  bs_scope_destroy(scope);
  bs_ast_destroy(ast);
  // TODO: Return the actual result of the program
  // Something like return (int)result.data
  return 0;
}
