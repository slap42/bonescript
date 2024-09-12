cmake_minimum_required(VERSION 3.10)
project(Bonescript)

# bonescript
set(BONESCRIPT_INCLUDE_FILES
  bonescript/include/bonescript/bonescript.h
)
source_group("include" FILES ${BONESCRIPT_INCLUDE_FILES})

set(BONESCRIPT_SRC_FILES
  bonescript/src/bonescript.c
)
source_group("src" FILES ${BONESCRIPT_SRC_FILES})

set(BONESCRIPT_SRC_LEXER_FILES
  bonescript/src/lexer/lexer.h
  bonescript/src/lexer/lexer.c
  bonescript/src/lexer/token.h
  bonescript/src/lexer/token.c
)
source_group("src/lexer" FILES ${BONESCRIPT_SRC_LEXER_FILES})

set(BONESCRIPT_FILES
  ${BONESCRIPT_INCLUDE_FILES}
  ${BONESCRIPT_SRC_FILES}
  ${BONESCRIPT_SRC_LEXER_FILES}
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
)
source_group("code" FILES ${BONESCRIPT_TESTS_CODE_FILES})
set_source_files_properties(${BONESCRIPT_TESTS_CODE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(bonescript-tests ${BONESCRIPT_TESTS_FILES} ${BONESCRIPT_TESTS_CODE_FILES})
target_link_libraries(bonescript-tests PRIVATE bonescript)
