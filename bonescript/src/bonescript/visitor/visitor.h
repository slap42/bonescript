#ifndef BONESCRIPT_VISITOR_H_
#define BONESCRIPT_VISITOR_H_

#include "bonescript/ast/ast.h"
#include "bonescript/visitor/scope.h"
#include "bonescript/visitor/variable.h"

bs_variable_t* bs_visit(bs_ast_t* ast, bs_scope_t* scope);

#endif
