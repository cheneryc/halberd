project(halberd_parser_test)

set(SOURCE src/parser_test.cpp
           src/parse_result_test.cpp
           src/combinator_test_common.h
           src/combinator_optional_test.cpp
           src/combinator_repeat_test.cpp
           src/source_test.cpp
           src/source_buffer_test.cpp)

add_executable(halberd_parser_test ${SOURCE})

target_link_libraries(halberd_parser_test PUBLIC halberd_parser gtest gtest_main gmock)

target_compile_features(halberd_parser_test PUBLIC cxx_std_14)
set_target_properties(halberd_parser_test PROPERTIES CXX_EXTENSIONS OFF)
