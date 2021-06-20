project(halberd_compiler)

set(INCLUDE include)

set(HEADER include/halberd/compiler.h
           include/halberd/rule.h
           include/halberd/rule_common.h
           include/halberd/rule_expression.h
           include/halberd/rule_variable_declaration.h
           include/halberd/expression_postfix_transform.h
           include/halberd/expression_prefix_transform.h
           include/halberd/variable_declaration_transform.h
           include/halberd/util/type.h)

set(SOURCE src/compiler.cpp
           src/expression_postfix_transform.cpp
           src/expression_prefix_transform.cpp
           src/variable_declaration_transform.cpp)

add_library(halberd_compiler STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_compiler PUBLIC ${INCLUDE})

target_link_libraries(halberd_compiler PUBLIC halberd_lexer halberd_parser halberd_syntax)

target_compile_features(halberd_compiler PUBLIC cxx_std_14)
set_target_properties(halberd_compiler PROPERTIES CXX_EXTENSIONS OFF)
