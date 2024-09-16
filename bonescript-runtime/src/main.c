#include <stdio.h>
#include <stdlib.h>
#include <bonescript/bonescript.h>
#include <bonescript/error.h>

static char* load_file(const char* path) {
  FILE* file = fopen(path, "rb");
  if (!file) return NULL;
  fseek(file, 0L, SEEK_END);
  size_t size = ftell(file);
  char* buf = malloc(size + 1);
  rewind(file);
  fread(buf, 1, size, file);
  fclose(file);
  buf[size] = '\0';
  return buf;
}

static void error_callback(bs_error_t error_code, const char* error_string) {
  fprintf(stderr, "Bonescript error: %s\n", error_string);
  exit(1);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "No input file");
    return 1;
  }

  char* code = load_file(argv[1]);
  if (!code) {
    fprintf(stderr, "Failed to open file %s", argv[1]);
    return 1;
  }

  bs_error_set_callback(error_callback);
  int result = bs_execute(code);
  free(code);
  return result;
}
