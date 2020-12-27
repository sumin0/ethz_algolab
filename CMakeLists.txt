# Example CMakeLists

cmake_minimum_required(VERSION 3.16)
project(algolab)

set(CMAKE_CXX_STANDARD 14)

find_package(CGAL)

# Add the following line for each source code
# add_executable($EXAMPLE src/$EXAMPLE.cpp)

# Additional line for those that require CGAL library
# target_link_libraries(maximizeit CGAL::CGAL)
