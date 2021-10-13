project(halberd_util)

set(INCLUDE include)

set(HEADER include/halberd/util/enum_flags.h
           include/halberd/util/optional.h
           include/halberd/util/overload.h
           include/halberd/util/string.h
           include/halberd/util/type.h)

add_library(halberd_util INTERFACE)

target_sources(halberd_util INTERFACE ${HEADER}) # required to associate files with an INTERFACE library

target_include_directories(halberd_util INTERFACE ${INCLUDE})

target_compile_features(halberd_util INTERFACE cxx_std_14)
