cmake_minimum_required(VERSION 3.11)

project(fuzz LANGUAGES C CXX)

add_executable(
    css_parser_fuzz
    css_parser_fuzz.cpp
)

set_target_properties(css_parser_fuzz PROPERTIES
    CXX_STANDARD 14
    C_STANDARD 99
)

target_include_directories(
    css_parser_fuzz
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml
)

target_link_libraries(
    css_parser_fuzz
    litehtml
)
