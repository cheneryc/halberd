project(halberd_lexer)

set(INCLUDE include)

set(HEADER include/halberd/lexer.h
           include/halberd/symbol_set.h
           include/halberd/symbol_set_array.h
           include/halberd/symbol_set_operators.h
           include/halberd/meta/dependent.h
           include/halberd/meta/value_list.h
           include/halberd/meta/value_set.h)

set(SOURCE src/lexer.cpp)

add_library(halberd_lexer STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_lexer PUBLIC ${INCLUDE})

target_compile_features(halberd_lexer PUBLIC cxx_std_14)
set_target_properties(halberd_lexer PROPERTIES CXX_EXTENSIONS OFF)
