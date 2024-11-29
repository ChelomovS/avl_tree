cmake_minimum_required(VERSION 3.11)

project(my_avl_tree)

set(SOURCES
        avl_tree.cxx
)
set (HEADERS
        avl_tree.hxx
)

add_executable(avl_tree ${SOURCES} ${HEADERS})

set_target_properties(
        avl_tree PROPERTIES 
        CXX_STANDART 20
        CXX_STANDARD_REQUIRED ON
)