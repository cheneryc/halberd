project(halberd_compiler)

set(INCLUDE include)

set(HEADER include/halberd/compiler.h
           include/halberd/rule.h
           include/halberd/rule_common.h
           include/halberd/rule_expression.h
           include/halberd/rule_statement.h
           include/halberd/rule_variable_declaration.h
           include/halberd/transform/transform_declaration_statement.h
           include/halberd/transform/transform_expression_statement.h
           include/halberd/transform/transform_literal.h
           include/halberd/transform/transform_operator_assignment.h
           include/halberd/transform/transform_operator_binary.h
           include/halberd/transform/transform_operator_unary_postfix.h
           include/halberd/transform/transform_operator_unary_prefix.h
           include/halberd/transform/transform_statement_list.h
           include/halberd/transform/transform_variable_declaration.h
           include/halberd/util/type.h)

set(SOURCE src/compiler.cpp
           src/transform/transform_expression_statement.cpp
           src/transform/transform_literal.cpp
           src/transform/transform_operator_assignment.cpp
           src/transform/transform_operator_binary.cpp
           src/transform/transform_operator_unary_postfix.cpp
           src/transform/transform_operator_unary_prefix.cpp
           src/transform/transform_statement_list.cpp
           src/transform/transform_variable_declaration.cpp)

add_library(halberd_compiler STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_compiler PUBLIC ${INCLUDE})

target_link_libraries(halberd_compiler PUBLIC halberd_lexer halberd_parser halberd_syntax)

target_compile_features(halberd_compiler PUBLIC cxx_std_14)
set_target_properties(halberd_compiler PROPERTIES CXX_EXTENSIONS OFF)
