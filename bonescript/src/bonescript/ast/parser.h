#ifndef BONESCRIPT_PARSER_H_
#define BONESCRIPT_PARSER_H_

#include "bonescript/ast/ast.h"

bs_ast_t* bs_parse_tokens(const char* code);

#endif
