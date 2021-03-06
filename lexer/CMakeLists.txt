project(halberd_lexer)

set(INCLUDE include)

set(HEADER include/halberd/keyword.h
           include/halberd/lexer.h
           include/halberd/lexer_tag.h
           include/halberd/scan.h
           include/halberd/scanner.h
           include/halberd/state_index.h
           include/halberd/state_machine.h
           include/halberd/state_machine_array.h
           include/halberd/state_machine_operators.h
           include/halberd/state_machine_runner.h
           include/halberd/symbol.h
           include/halberd/symbol_set.h
           include/halberd/symbol_set_array.h
           include/halberd/symbol_set_operators.h
           include/halberd/token.h
           include/halberd/meta/dependent.h
           include/halberd/meta/type_list.h
           include/halberd/meta/type_wrapper.h
           include/halberd/meta/value_list.h
           include/halberd/meta/value_list_transform.h
           include/halberd/meta/value_set.h
           include/halberd/meta/value_wrapper.h
           include/halberd/util/string.h)

set(SOURCE src/keyword.cpp
           src/lexer.cpp
           src/scan.cpp
           src/scanner.cpp
           src/symbol.cpp
           src/token.cpp)

add_library(halberd_lexer STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_lexer PUBLIC ${INCLUDE})

target_compile_features(halberd_lexer PUBLIC cxx_std_14)
set_target_properties(halberd_lexer PROPERTIES CXX_EXTENSIONS OFF)
