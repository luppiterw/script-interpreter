cmake_minimum_required(VERSION 3.14)
project(ScriptInterpreter VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
)

# Library files
set(LIBRARY_SOURCES
    src/token.cpp
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
)

# Create library
add_library(script_interpreter_lib ${LIBRARY_SOURCES})

# Create executable
add_executable(script_interpreter ${SOURCES})
target_link_libraries(script_interpreter script_interpreter_lib)

# Test executables
add_executable(lexer_test tests/lexer_test.cpp)
target_link_libraries(lexer_test script_interpreter_lib)

add_executable(lexer_extended_test tests/lexer_extended_test.cpp)
target_link_libraries(lexer_extended_test script_interpreter_lib)

add_executable(parser_test tests/parser_test.cpp)
target_link_libraries(parser_test script_interpreter_lib)

add_executable(evaluator_test tests/evaluator_test.cpp)
target_link_libraries(evaluator_test script_interpreter_lib)

add_executable(evaluator_extended_test tests/evaluator_extended_test.cpp)
target_link_libraries(evaluator_extended_test script_interpreter_lib)

# Installation
install(TARGETS script_interpreter DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

# Enable testing
enable_testing()

# Add tests
add_test(NAME lexer_test COMMAND lexer_test)
add_test(NAME lexer_extended_test COMMAND lexer_extended_test)
add_test(NAME parser_test COMMAND parser_test)
add_test(NAME evaluator_test COMMAND evaluator_test)
add_test(NAME evaluator_extended_test COMMAND evaluator_extended_test)