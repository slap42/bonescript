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

static bs_variable_t* bs_std_add(bs_ast_t** args, size_t args_count, bs_scope_t* scope) {
  if (args_count != 2) {
    BS_ERROR("add() requires exactly 2 arguments");
  }
  bs_variable_t* a = bs_visit(args[0], scope);
  bs_variable_t* b = bs_visit(args[1], scope);
  if (a->type != BS_VARIABLE_TYPE_INT || b->type != BS_VARIABLE_TYPE_INT) {
    BS_ERROR("add() requires 2 ints for arguments");
  }
  int aa = *(int*)a->data;
  int bb = *(int*)b->data;
  int r = aa + bb;
  bs_variable_t* result = bs_variable_create(BS_VARIABLE_TYPE_INT);
  bs_variable_assign_data(result, &r, sizeof(int));
  bs_variable_destroy(a);
  bs_variable_destroy(b);
  return result;
}

static void bs_import_std_math(bs_scope_t* scope) {
  // add
  const char* BS_ADD_FUNCTION_NAME = "add";
  bs_variable_t* add = bs_variable_create(BS_VARIABLE_TYPE_BUILTIN_FUNCTION);
  bs_variable_assign_name(add, BS_ADD_FUNCTION_NAME, strlen(BS_ADD_FUNCTION_NAME));
  add->data = (char*)bs_std_add;
  bs_scope_store_variable(scope, add);
}

void bs_import_symbols(bs_scope_t* scope, bs_variable_t* var) {
  // Importing standard library symbols
  if (strcmp(var->data, "std-io") == 0) {
    bs_import_std_io(scope);
  }
  else if (strcmp(var->data, "std-math") == 0) {
    bs_import_std_math(scope);
  }
  // Reading a file and importing the symbols it defines
  else {
    // TODO: Reading files and importing symbols
    BS_ERROR("Sorry, we can't read files yet");
  }
}
