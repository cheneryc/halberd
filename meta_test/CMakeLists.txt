project(halberd_meta_test)

set(SOURCE src/type_list_test.cpp)

add_executable(halberd_meta_test ${SOURCE})

target_link_libraries(halberd_meta_test PUBLIC halberd_meta gtest gtest_main gmock)

target_compile_features(halberd_meta_test PUBLIC cxx_std_14)
set_target_properties(halberd_meta_test PROPERTIES CXX_EXTENSIONS OFF)
