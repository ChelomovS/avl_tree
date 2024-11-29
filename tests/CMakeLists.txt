cmake_minimum_required(VERSION 3.11)

project(unit_tests)

if(NOT DEFINED INCLUDE_DIR)
    message(WARNING "INCLUDE_DIR is not defined.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

aux_source_directory(unit UNIT_TESTS)

add_executable(unit_tests ${UNIT_TESTS})

target_include_directories(unit_tests PRIVATE ${INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})

find_package(GTest REQUIRED)
enable_testing()

target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES})

add_test(NAME unit_tests COMMAND unit_tests)