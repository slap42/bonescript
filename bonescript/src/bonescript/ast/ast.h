#ifndef BONESCRIPT_AST_H_
#define BONESCRIPT_AST_H_

#include <stddef.h>

typedef struct BS_AST_STRUCT {

  enum BS_AST_TYPE {
    BS_AST_COMPOUND,
    BS_AST_STRING_VARIABLE_DEFINITION,
    BS_AST_VARIABLE,
    BS_AST_FUNCTION_CALL,
    BS_AST_STRING,
  } type;

  union {

    struct {
      struct BS_AST_STRUCT** value;
      size_t size;
    } compound;

    struct {
      const char* name;
      size_t name_length;
      struct BS_AST_STRUCT* value;
    } string_variable_definition;

    struct {
      const char* name;
      size_t name_length;
    } variable;

    struct {
      const char* value;
      size_t value_length;
    } string;

/*
    struct {
      char* name;
      struct BS_AST_STRUCT** args;
      size_t args_count;
    } function_call;
*/
  };

} bs_ast_t;

bs_ast_t* bs_ast_create(int type);
void bs_ast_destroy(bs_ast_t* ast);
void bs_ast_print(bs_ast_t* ast);

#endif
