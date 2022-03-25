project(halberd_meta)

set(INCLUDE include)

set(HEADER include/halberd/meta/dependent.h
           include/halberd/meta/static_if.h
           include/halberd/meta/type_list.h
           include/halberd/meta/type_list_transform.h
           include/halberd/meta/type_wrapper.h
           include/halberd/meta/value_list.h
           include/halberd/meta/value_list_transform.h
           include/halberd/meta/value_set.h
           include/halberd/meta/value_wrapper.h)

add_library(halberd_meta INTERFACE)

target_sources(halberd_meta INTERFACE ${HEADER}) # required to associate files with an INTERFACE library

target_include_directories(halberd_meta INTERFACE ${INCLUDE})

target_compile_features(halberd_meta INTERFACE cxx_std_14)
