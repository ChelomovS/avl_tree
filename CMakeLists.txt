cmake_minimum_required(VERSION 3.11)

project(avl_tree)

find_package(GTest REQUIRED)
enable_testing()

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/my_avl_tree) 

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -Wpedantic -g -O0 -D DEBUG 
                           -Wmissing-declarations -Wcast-align 
                           -Wunnused -Wshadow --Wsign-conversion
                           -fsanitize=address -fsanitize=undefined")

add_subdirectory(standard_set)
add_subdirectory(my_avl_tree)
add_subdirectory(tests)