project(halberd_syntax)

set(INCLUDE include)

set(HEADER include/halberd/expression.h
           include/halberd/node.h
           include/halberd/operator_binary.h
           include/halberd/operator_unary_postfix.h
           include/halberd/operator_unary_prefix.h
           include/halberd/tree.h
           include/halberd/tree_test.h
           include/halberd/variable_declaration.h)

set(SOURCE src/expression.cpp
           src/node.cpp
           src/operator_binary.cpp
           src/operator_unary_postfix.cpp
           src/operator_unary_prefix.cpp
           src/tree.cpp
           src/variable_declaration.cpp)

add_library(halberd_syntax STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_syntax PUBLIC ${INCLUDE})

target_compile_features(halberd_syntax PUBLIC cxx_std_14)
set_target_properties(halberd_syntax PROPERTIES CXX_EXTENSIONS OFF)
