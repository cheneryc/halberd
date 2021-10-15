project(halberd_syntax)

set(INCLUDE include)

set(HEADER include/halberd/const_visit.h
           include/halberd/const_visitor.h
           include/halberd/const_visitor_traversal.h
           include/halberd/declaration.h
           include/halberd/declaration_statement.h
           include/halberd/expression.h
           include/halberd/expression_statement.h
           include/halberd/identifier_expression.h
           include/halberd/list.h
           include/halberd/literal.h
           include/halberd/node.h
           include/halberd/operator_assignment.h
           include/halberd/operator_base.h
           include/halberd/operator_binary.h
           include/halberd/operator_unary_postfix.h
           include/halberd/operator_unary_prefix.h
           include/halberd/statement.h
           include/halberd/statement_list.h
           include/halberd/tree.h
           include/halberd/tree_test.h
           include/halberd/type.h
           include/halberd/variable_declaration.h
           include/halberd/variable_declaration_statement.h
           include/halberd/variable_identifier.h
           include/halberd/visit.h
           include/halberd/visitor.h
           include/halberd/visitor_antecedent.h
           include/halberd/visitor_function.h
           include/halberd/visitor_reset.h
           include/halberd/visitor_traversal.h)

set(SOURCE src/const_visit.cpp
           src/const_visitor_traversal.cpp
           src/declaration.cpp
           src/declaration_statement.cpp
           src/expression.cpp
           src/expression_statement.cpp
           src/identifier_expression.cpp
           src/literal.cpp
           src/node.cpp
           src/operator_assignment.cpp
           src/operator_base.cpp
           src/operator_binary.cpp
           src/operator_unary_postfix.cpp
           src/operator_unary_prefix.cpp
           src/statement_list.cpp
           src/tree.cpp
           src/variable_declaration.cpp
           src/variable_declaration_statement.cpp
           src/variable_identifier.cpp
           src/visit.cpp
           src/visitor_antecedent.cpp
           src/visitor_traversal.cpp)

add_library(halberd_syntax STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_syntax PUBLIC ${INCLUDE})

target_link_libraries(halberd_syntax PUBLIC halberd_util)

target_compile_features(halberd_syntax PUBLIC cxx_std_14)
set_target_properties(halberd_syntax PROPERTIES CXX_EXTENSIONS OFF)
