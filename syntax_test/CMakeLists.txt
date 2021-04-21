project(halberd_syntax_test)

set(SOURCE src/expression_test.cpp)

add_executable(halberd_syntax_test ${SOURCE})

target_link_libraries(halberd_syntax_test PUBLIC halberd_syntax gtest gtest_main gmock)

target_compile_features(halberd_syntax_test PUBLIC cxx_std_14)
set_target_properties(halberd_syntax_test PROPERTIES CXX_EXTENSIONS OFF)
