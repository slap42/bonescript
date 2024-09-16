#include "bonescript/visitor/import.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bonescript/error/error.h"
#include "bonescript/visitor/function.h"
#include "bonescript/visitor/visitor.h"

static bs_variable_t* bs_std_print(bs_ast_t** args, size_t args_count, bs_scope_t* scope) {
  for (size_t i = 0; i < args_count; ++i) {
    bs_variable_t* var = bs_visit(args[i], scope);
    switch (var->type) {
      case BS_VARIABLE_TYPE_STRING: printf("%s", (char*)var->data); break;
      case BS_VARIABLE_TYPE_INT:    printf("%d", *(int*)var->data); break;
      case BS_VARIABLE_TYPE_FLOAT:  printf("%f", *(float*)var->data); break;
      default: BS_ERROR("Tried to print invalid variable type: %d", var->type); break;
    }
    if (!var->name) {
      bs_variable_destroy(var);
    }
  }
  printf("\n");
  return NULL;
}

static void bs_import_std_io(bs_scope_t* scope) {
  // print
  const char* PRINT_FUNCTION_NAME = "print";
  bs_variable_t* print = bs_variable_create(BS_VARIABLE_TYPE_BUILTIN_FUNCTION);
  bs_variable_assign_name(print, PRINT_FUNCTION_NAME, strlen(PRINT_FUNCTION_NAME));
  print->data = (char*)bs_std_print;
  bs_scope_store_variable(scope, print);
}

void bs_import_symbols(bs_scope_t* scope, bs_variable_t* var) {
  // Importing standard library symbols
  if (strcmp(var->data, "std-io") == 0) {
    bs_import_std_io(scope);
  }
  // Reading a file and importing the symbols it defines
  else {
    // TODO: Reading files and importing symbols
  }
}
