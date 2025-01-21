# CMakeList.txt : CMake project for binary_search_tree, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (binary_search_tree "binary_search_tree.cpp" "binary_search_tree.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET binary_search_tree PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
