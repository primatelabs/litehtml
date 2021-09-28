cmake_minimum_required(VERSION 3.11)

project(headless LANGUAGES C CXX)
include(CTest)
enable_testing()

find_package(PkgConfig)
pkg_search_module(CAIRO REQUIRED cairo>=1.12.0)
pkg_search_module(FONTCONFIG REQUIRED fontconfig>=2.0.0)
set(HEADLESS_NAME headless)
add_executable(
    ${HEADLESS_NAME}
    ${SOURCE_HEADLESS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc
)
set_target_properties(${HEADLESS_NAME} PROPERTIES
    CXX_STANDARD 11
    C_STANDARD 99
)
target_include_directories(
    ${HEADLESS_NAME}
    PUBLIC
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
    ${PROJECT_NAME}
    ${CAIRO_LIBRARIES}
    ${FONTCONFIG_LIBRARIES}
)
