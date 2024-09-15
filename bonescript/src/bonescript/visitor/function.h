#ifndef BONESCRIPT_FUNCTION_H_
#define BONESCRIPT_FUNCTION_H_

#include <stddef.h>
#include "bonescript/ast/ast.h"
#include "bonescript/visitor/scope.h"
#include "bonescript/visitor/visitor.h"

typedef bs_variable_t*(*bs_function_ptr_t)(bs_ast_t** args, size_t args_count, bs_scope_t* scope);

#endif
