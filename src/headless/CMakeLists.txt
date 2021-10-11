cmake_minimum_required(VERSION 3.11)

project(headless LANGUAGES C CXX)
include(CTest)
enable_testing()

find_package(PkgConfig)
pkg_search_module(CAIRO REQUIRED cairo>=1.12.0)
pkg_search_module(FONTCONFIG REQUIRED fontconfig>=2.0.0)

find_library(FOUNDATION Foundation)

set(HEADLESS_NAME headless)

set(SOURCE_HEADLESS
    headless.cpp
    headless_container.cpp
    http_darwin.mm
    http.cpp
)

add_executable(
    ${HEADLESS_NAME}
    ${SOURCE_HEADLESS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/master.css.inc
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/master.css.inc PROPERTIES GENERATED TRUE)

set_target_properties(${HEADLESS_NAME} PROPERTIES
    CXX_STANDARD 11
    C_STANDARD 99
)

target_include_directories(
    ${HEADLESS_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml
    ${CAIRO_INCLUDE_DIRS}
    ${FONTCONFIG_INCLUDE_DIRS}
)

target_link_directories(
    ${HEADLESS_NAME}
    PUBLIC
    ${CAIRO_LIBRARY_DIRS}
    ${FONTCONFIG_LIBRARY_DIRS}
)

target_link_libraries(
    ${HEADLESS_NAME}
    litehtml
    ${CAIRO_LIBRARIES}
    ${FONTCONFIG_LIBRARIES}
    ${FOUNDATION}
)
