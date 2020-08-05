project(halberd_parser)

set(INCLUDE include)

set(HEADER include/halberd/parser.h)

set(SOURCE src/parser.cpp)

add_library(halberd_parser STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_parser PUBLIC ${INCLUDE})

target_link_libraries(halberd_parser PUBLIC halberd_lexer)

target_compile_features(halberd_parser PUBLIC cxx_std_14)
set_target_properties(halberd_parser PROPERTIES CXX_EXTENSIONS OFF)
