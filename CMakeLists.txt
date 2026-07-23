cmake_minimum_required(VERSION 3.1)

set(EXPORT_COMPILE_COMMANDS ON)

project(kalkcliator)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/fn_registry.h
    src/tokens.h
    src/tree.h
    src/lexer.h
    src/parser.h
)
