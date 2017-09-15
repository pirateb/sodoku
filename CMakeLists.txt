cmake_minimum_required(VERSION 2.8)

project(sodoku)
# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# c++14
set(CMAKE_CXX_STANDARD 14)

# Set sources etc
file(GLOB_RECURSE sources      ${PROJECT_SOURCE_DIR}/src/main/*.cpp src/main/*.h)
file(GLOB_RECURSE sources_test ${PROJECT_SOURCE_DIR}/src/test/*.cpp)
# file(GLOB_RECURSE data resources/*)
set(source_main ${PROJECT_SOURCE_DIR}/src/main.cpp)

# Set where modules come from
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/src/main/")

# debug build / can toggle
add_definitions(-DDEBUG)
add_executable(sodoku ${source_main} ${sources})
target_compile_definitions(sodoku PUBLIC DEBUG)

# Tests
include(GoogleTest)
add_executable(unit_tests ${sources_test} ${sources})
target_compile_definitions(unit_tests PUBLIC UNIT_TESTS)
set_target_properties(sodoku PROPERTIES ENABLE_EXPORTS on) # so that we can link
target_link_libraries(unit_tests PUBLIC
    ${GoogleTestLibraries}
    sodoku
)

target_include_directories(unit_tests PUBLIC
    ${GoogleTestIncludes}
)

