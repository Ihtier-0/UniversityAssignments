cmake_minimum_required(VERSION 3.14)

project(CuttingPlaneMethod LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("${CMAKE_CXX_COMPILER_ID} COMPILER")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  string(REGEX REPLACE "/W[3|4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  add_compile_options(/W4 /WX)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  add_compile_options(
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wenum-compare
    -Wfloat-equal
    -Wredundant-decls
    -Wsign-conversion
    # https://gcc.gnu.org/onlinedocs/gcc/C_002b_002b-Dialect-Options.html
    -Wctor-dtor-privacy
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wsign-promo
  )
endif()

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${HDRS} ${SRCS})
