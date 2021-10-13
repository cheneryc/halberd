cmake_minimum_required(VERSION 3.13)

project(halberd LANGUAGES CXX)

include(FetchContent)
include(cmake/FetchContent_MakeAvailable.cmake)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0)

FetchContent_MakeAvailable(googletest)

add_subdirectory(lexer)
add_subdirectory(lexer_test)
add_subdirectory(parser)
add_subdirectory(parser_test)
add_subdirectory(syntax)
add_subdirectory(syntax_test)
add_subdirectory(compiler)
add_subdirectory(compiler_test)
add_subdirectory(meta)
add_subdirectory(meta_test)
add_subdirectory(util)
