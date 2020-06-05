project(halberd_lexer_test)

set(SOURCE src/lexer_test.cpp
           src/state_machine_test.cpp
           src/state_machine_operators_test.cpp
           src/symbol_set_test.cpp
           src/symbol_set_operators_test.cpp)

add_executable(halberd_lexer_test ${SOURCE})

target_link_libraries(halberd_lexer_test halberd_lexer gtest gtest_main gmock)

target_compile_features(halberd_lexer_test PUBLIC cxx_std_14)
set_target_properties(halberd_lexer_test PROPERTIES CXX_EXTENSIONS OFF)
