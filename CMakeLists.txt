cmake_minimum_required(VERSION 3.10)
project(Bonescript)

# bonescript
set(BONESCRIPT_INCLUDE_FILES
  bonescript/include/bonescript/bonescript.h
  bonescript/include/bonescript/error.h
)
source_group("include/bonescript" FILES ${BONESCRIPT_INCLUDE_FILES})

set(BONESCRIPT_SRC_FILES
  bonescript/src/bonescript/bonescript.c
)
source_group("src/bonescript" FILES ${BONESCRIPT_SRC_FILES})

set(BONESCRIPT_SRC_LEXER_FILES
  bonescript/src/bonescript/lexer/lexer.h
  bonescript/src/bonescript/lexer/lexer.c
  bonescript/src/bonescript/lexer/token.h
  bonescript/src/bonescript/lexer/token.c
)
source_group("src/bonescript/lexer" FILES ${BONESCRIPT_SRC_LEXER_FILES})

set(BONESCRIPT_SRC_AST_FILES
  bonescript/src/bonescript/ast/ast.h
  bonescript/src/bonescript/ast/ast.c
  bonescript/src/bonescript/ast/parser.h
  bonescript/src/bonescript/ast/parser.c
)
source_group("src/bonescript/ast" FILES ${BONESCRIPT_SRC_AST_FILES})

set(BONESCRIPT_SRC_ERROR_FILES
  bonescript/src/bonescript/error/error_internal.h
  bonescript/src/bonescript/error/error.c
)
source_group("src/bonescript/error" FILES ${BONESCRIPT_SRC_ERROR_FILES})

set(BONESCRIPT_FILES
  ${BONESCRIPT_INCLUDE_FILES}
  ${BONESCRIPT_SRC_FILES}
  ${BONESCRIPT_SRC_LEXER_FILES}
  ${BONESCRIPT_SRC_AST_FILES}
  ${BONESCRIPT_SRC_ERROR_FILES}
)

add_library(bonescript ${BONESCRIPT_FILES})
target_include_directories(bonescript PUBLIC bonescript/include)
target_include_directories(bonescript PRIVATE bonescript/src)

# bonescript-tests
set(BONESCRIPT_TESTS_FILES
bonescript-tests/src/main.c
)
source_group("src" FILES ${BONESCRIPT_TESTS_FILES})

set(BONESCRIPT_TESTS_CODE_FILES
  bonescript-tests/code/print.bs
  bonescript-tests/code/test-lexer.bs
  bonescript-tests/code/test-parser.bs
)
source_group("code" FILES ${BONESCRIPT_TESTS_CODE_FILES})
set_source_files_properties(${BONESCRIPT_TESTS_CODE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(bonescript-tests ${BONESCRIPT_TESTS_FILES} ${BONESCRIPT_TESTS_CODE_FILES})
target_link_libraries(bonescript-tests PRIVATE bonescript)
target_include_directories(bonescript-tests PRIVATE bonescript/src)
if(WIN32)
  set_target_properties(bonescript-tests PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bonescript-tests")
endif()
