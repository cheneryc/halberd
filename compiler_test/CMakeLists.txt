project(halberd_compiler_test)

set(SOURCE src/compiler_test.cpp
           src/compiler_test_common.h
           src/compile_expression_test.cpp
           src/compile_statement_test.cpp)

add_executable(halberd_compiler_test ${SOURCE})

target_link_libraries(halberd_compiler_test PUBLIC halberd_compiler gtest gtest_main gmock)

target_compile_features(halberd_compiler_test PUBLIC cxx_std_14)
set_target_properties(halberd_compiler_test PROPERTIES CXX_EXTENSIONS OFF)
