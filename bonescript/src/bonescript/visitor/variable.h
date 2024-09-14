#ifndef BONESCRIPT_VARIABLE_H_
#define BONESCRIPT_VARIABLE_H_

#include <stddef.h>

typedef struct BS_VARIABLE_STRUCT {
  
  enum {
    BS_VARIABLE_STRING,
  } type;
  
  char* name;
  
  union {
   
    struct {
      char* str;
    } string;
    
  };
  
} bs_variable_t;

bs_variable_t* bs_variable_string_create(const char* name, size_t name_length, const char* str, size_t str_length);
void bs_variable_destroy(bs_variable_t* variable);

#endif
