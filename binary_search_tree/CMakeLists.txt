# CMakeList.txt : CMake project for binary_search_tree, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (binary_search_tree "src/client.cpp" "src/binary_search_tree.h" "src/tree.h" "src/tree.cpp" "src/bst_test.cpp" "src/avl_tree_test.cpp")



if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET binary_search_tree PROPERTY CXX_STANDARD 20)
endif()

add_executable(client src/client.cpp)
# TODO: Add tests and install targets if needed.
enable_testing()
add_library(catch2_main lib/catch2_main.cpp)

target_include_directories(catch2_main PUBLIC lib)
# link the library so that other programs can get it
link_libraries(catch2_main)

add_executable(bst_test_exe src/bst_test.cpp)
add_test(bst_test bst_test_exe)
