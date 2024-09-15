#ifndef BONESCRIPT_IMPORT_H_
#define BONESCRIPT_IMPORT_H_

#include "bonescript/visitor/scope.h"
#include "bonescript/visitor/variable.h"

void bs_import_symbols(bs_scope_t* scope, bs_variable_t* var);

#endif
