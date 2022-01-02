cmake_minimum_required(VERSION 3.11)

project(litehtml LANGUAGES C CXX)
include(CTest)
enable_testing()

option(BUILD_FUZZ "Build fuzzing executables" OFF)
option(BUILD_HEADLESS "Build headless litehtml browser" OFF)
option(EXTERNAL_GUMBO "Link against external gumbo instead of shipping a bundled copy" OFF)

if(NOT EXTERNAL_GUMBO)
add_subdirectory(third_party/gumbo)
endif()

add_subdirectory(src/litehtml)

if(BUILD_FUZZ)
add_subdirectory(src/fuzz)
endif()

if(BUILD_HEADLESS)
add_subdirectory(src/headless)
endif()
