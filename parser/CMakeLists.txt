project(halberd_parser)

set(INCLUDE include)

set(HEADER include/halberd/combinator.h
           include/halberd/combinator_choice.h
           include/halberd/combinator_end.h
           include/halberd/combinator_filter.h
           include/halberd/combinator_one.h
           include/halberd/combinator_select.h
           include/halberd/combinator_sequence.h
           include/halberd/combinator_transform.h
           include/halberd/parser.h
           include/halberd/parse_result.h
           include/halberd/source.h
           include/halberd/source_buffer.h
           include/halberd/util/enum_flags.h
           include/halberd/util/optional.h)

set(SOURCE src/parser.cpp)

add_library(halberd_parser STATIC ${SOURCE} ${HEADER})

target_include_directories(halberd_parser PUBLIC ${INCLUDE})

target_compile_features(halberd_parser PUBLIC cxx_std_14)
set_target_properties(halberd_parser PROPERTIES CXX_EXTENSIONS OFF)
