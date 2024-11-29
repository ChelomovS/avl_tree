cmake_minimum_required(VERSION 3.11)

project(standart_set)

set(SOURCES
        set.cxx
)

add_executable(set ${SOURCES} ${HEADERS})

set_target_properties(
        set PROPERTIES 
        CXX_STANDART 20
        CXX_STANDARD_REQUIRED ON
)