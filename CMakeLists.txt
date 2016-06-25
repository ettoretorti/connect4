cmake_minimum_required (VERSION 3.1)
project (connect4)

option(BUILD_TESTS "Build the unit tests" OFF)
option(USE_OPENMP "Parallelize some of the AI with OpenMP" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.c")

add_library(con4 ${SOURCE_FILES})

target_include_directories(
    con4
    PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_compile_options(
    con4
    PRIVATE -Wall)

if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    #g++ options
    target_compile_options(
        con4
        PRIVATE -Wextra
        PRIVATE -pedantic)
endif()

add_executable(con4game "")
target_link_libraries(con4game con4)


if(USE_OPENMP)
    find_package(OpenMP REQUIRED)
    target_compile_options(
        con4
        PRIVATE ${OpenMP_CXX_FLAGS})

    if(CMAKE_COMPILER_IS_GNUCXX)
        target_link_libraries(
            con4
            PRIVATE gomp)
    endif()
endif()


if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    
    add_executable(alltests "test/all_tests.cpp")
    target_include_directories(
        alltests
        PRIVATE ${CMAKE_SOURCE_DIR}/src
        PRIVATE ${CMAKE_SOURCE_DIR}/test)

    target_link_libraries(alltests con4 ${GTEST_BOTH_LIBRARIES})

    add_test(NAME alltests COMMAND alltests)

endif()
