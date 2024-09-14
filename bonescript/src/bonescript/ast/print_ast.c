#include "bonescript/ast/print_ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <windows.h>
#endif

typedef struct {
  char* buf;
  size_t size;
  size_t index;
} indent_t;

static indent_t* indent_create() {
  indent_t* indent = malloc(sizeof(indent_t));
  indent->size = 1;
  indent->buf = malloc(indent->size);
  indent->index = 0;
  return indent;
}

static void indent_destroy(indent_t* indent) {
  free(indent->buf);
  free(indent);
}

static void indent_print(indent_t* indent) {
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif
  for (size_t i = 0; i < indent->index; ++i) {
    if (indent->buf[i] == 'c') printf("├");
    if (indent->buf[i] == 'e') printf("└");
    if (indent->buf[i] == 'p') printf("│");
    if (indent->buf[i] == ' ') printf(" ");
  }
#ifdef _WIN32
  SetConsoleOutputCP(CP_ACP);
#endif
}

static void indent_push(indent_t* indent, bool last_child) {
  if (indent->index > 0) {
    if (indent->buf[indent->index - 1] == 'c') {
      indent->buf[indent->index - 1] = 'p';
    }
    else if (indent->buf[indent->index - 1] == 'e') {
      indent->buf[indent->index - 1] = ' ';
    }
  }
  
  indent->buf[indent->index] = last_child ? 'e' : 'c';
  indent->index++;
  if (indent->index> indent->size) {
    indent->size++;
    indent->buf = realloc(indent->buf, indent->size);
  }
}
      
static void indent_pop(indent_t* indent) {
  if (indent->index > 0) {
    if (indent->buf[indent->index - 1] == 'p') {
      indent->buf[indent->index - 1] = 'c';
    }
  }
  
  indent->index--;
}

static void bs_ast_print_node(bs_ast_t* ast, indent_t* indent);

static void bs_ast_print_string_literal(bs_ast_t* ast, indent_t* indent) {
  printf("<string literal>\n");
  indent_push(indent, true);
    indent_print(indent);
    printf("value: %.*s\n", (int)ast->string_literal.value_length, ast->string_literal.value);
  indent_pop(indent);
}

static void bs_ast_print_variable_definition(bs_ast_t* ast, indent_t* indent) {
  printf("<variable definition>\n");
  indent_push(indent, false);
    indent_print(indent);
    printf("type: %.*s\n", (int)ast->variable_definition.type_length, ast->variable_definition.type);
    indent_print(indent);
    printf("name: %.*s\n", (int)ast->variable_definition.name_length, ast->variable_definition.name);
  indent_pop(indent);
  indent_push(indent, true);
    indent_print(indent);
    printf("value:\n");
    indent_push(indent, true);
      bs_ast_print_node(ast->variable_definition.value, indent);
    indent_pop(indent);
  indent_pop(indent);
}

static void bs_ast_print_compound(bs_ast_t* ast, indent_t* indent) {
  printf("<compound>\n");
  for (size_t i = 0; i < ast->compound.size; ++i) {
    indent_push(indent, i == ast->compound.size - 1);
      bs_ast_print_node(ast->compound.value[i], indent);
    indent_pop(indent);
  }
}

static void bs_ast_print_function_call(bs_ast_t* ast, indent_t* indent) {
  printf("<function call>\n");
  indent_push(indent, false);
    indent_print(indent);
    printf("name: %.*s\n", (int)ast->function_call.name_length, ast->function_call.name);
  indent_pop(indent);
  if (ast->function_call.args_count > 0) {
    indent_push(indent, true);
      indent_print(indent);
      printf("args:\n");
      for (size_t i = 0; i < ast->function_call.args_count; ++i) {
        indent_push(indent, i == ast->function_call.args_count - 1);
          bs_ast_print_node(ast->function_call.args[i], indent);
        indent_pop(indent);
      }
    indent_pop(indent);
  }
  else {
    indent_push(indent, true);
      indent_print(indent);
      printf("args: empty\n");
    indent_pop(indent);
  }
}

static void bs_ast_print_variable(bs_ast_t* ast, indent_t* indent) {
  printf("<variable>\n");
  indent_push(indent, true);
    indent_print(indent);
    printf("name: %.*s\n", (int)ast->variable.name_length, ast->variable.name);
  indent_pop(indent);
}

void bs_ast_print_node(bs_ast_t* ast, indent_t* indent) {
  indent_print(indent);
  if (!ast) {
    printf("<NULL>\n");
    return;
  }
  switch(ast->type) {
    case BS_AST_COMPOUND:            bs_ast_print_compound(ast, indent); return;
    case BS_AST_VARIABLE_DEFINITION: bs_ast_print_variable_definition(ast, indent); return;
    case BS_AST_STRING_LITERAL:      bs_ast_print_string_literal(ast, indent); return;
    case BS_AST_FUNCTION_CALL:       bs_ast_print_function_call(ast, indent); return;
    case BS_AST_VARIABLE:            bs_ast_print_variable(ast, indent); return;
  }
}

void bs_ast_print(bs_ast_t* ast) {
  indent_t* indent = indent_create();
  bs_ast_print_node(ast, indent);
  indent_destroy(indent);
}
