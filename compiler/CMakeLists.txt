project(halberd_compiler)

set(INCLUDE include)

set(HEADER include/halberd/compiler.h
           include/halberd/util/type.h)

set(SOURCE src/compiler.cpp)

add_library(halberd_compiler STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_compiler PUBLIC ${INCLUDE})

target_link_libraries(halberd_compiler PUBLIC halberd_lexer halberd_parser)

target_compile_features(halberd_compiler PUBLIC cxx_std_14)
set_target_properties(halberd_compiler PROPERTIES CXX_EXTENSIONS OFF)
