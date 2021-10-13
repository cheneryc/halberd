project(halberd_parser)

set(INCLUDE include)

set(HEADER include/halberd/combinator.h
           include/halberd/combinator_any.h
           include/halberd/combinator_choice.h
           include/halberd/combinator_end.h
           include/halberd/combinator_filter.h
           include/halberd/combinator_function.h
           include/halberd/combinator_one.h
           include/halberd/combinator_optional.h
           include/halberd/combinator_repeat.h
           include/halberd/combinator_select.h
           include/halberd/combinator_sequence.h
           include/halberd/combinator_transform.h
           include/halberd/combinator_operators.h
           include/halberd/index_tag.h
           include/halberd/parser.h
           include/halberd/parse_result.h
           include/halberd/source.h
           include/halberd/source_buffer.h
           include/halberd/util/enum_flags.h)

set(SOURCE src/parser.cpp)

add_library(halberd_parser STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_parser PUBLIC ${INCLUDE})

target_link_libraries(halberd_parser PUBLIC halberd_util)

target_compile_features(halberd_parser PUBLIC cxx_std_14)
set_target_properties(halberd_parser PROPERTIES CXX_EXTENSIONS OFF)
