cmake_minimum_required(VERSION 3.11)

project(headless LANGUAGES C CXX)
include(CTest)
enable_testing()

include(FindJPEG)

find_package(PkgConfig)
pkg_search_module(FMT REQUIRED fmt)
pkg_search_module(FONTCONFIG REQUIRED fontconfig>=2.0.0)
pkg_search_module(FREETYPE REQUIRED freetype2)
pkg_search_module(HARFBUZZ REQUIRED harfbuzz)
pkg_search_module(LIBPNG REQUIRED libpng>=1.6.0)

set(HEADLESS_NAME headless)

set(SOURCE_HEADLESS
    flags.cpp
    font_description.cpp
    headless.cpp
    headless_container.cpp
    http.cpp
    image/composite.cpp
    image/convert.cpp
    image/image.cpp
    image/jpeg_codec.cpp
    image/png_codec.cpp
    orion_render_context.cpp
)

set(requiredlibs)

if (APPLE)
    find_library(FOUNDATION Foundation)
    list(APPEND SOURCE_HEADLESS http_darwin.mm)
    set(requiredlibs ${requiredlibs} ${FOUNDATION})
else()
    find_package(CURL)
    if(CURL_FOUND)
        list(APPEND SOURCE_HEADLESS http_curl.cpp)
        set(requiredlibs ${requiredlibs} ${CURL_LIBRARIES})
    endif(CURL_FOUND)
endif()

add_executable(
    ${HEADLESS_NAME}
    ${SOURCE_HEADLESS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/master.css.inc
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/master.css.inc PROPERTIES GENERATED TRUE)

set_target_properties(${HEADLESS_NAME} PROPERTIES
    CXX_STANDARD 17
    C_STANDARD 99
)

# target_compile_options(
#     ${HEADLESS_NAME}
#     PRIVATE
#     -fsanitize=address
# )
#
# target_link_options(
#     ${HEADLESS_NAME}
#     PRIVATE
#     -fsanitize=address
# )

target_include_directories(
    ${HEADLESS_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../litehtml
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/orion/include
    ${FONTCONFIG_INCLUDE_DIRS}
    ${JPEG_INCLUDE_DIRS}
)

target_link_directories(
    ${HEADLESS_NAME}
    PUBLIC
    ${HARFBUZZ_LIBRARY_DIRS}
    ${FONTCONFIG_LIBRARY_DIRS}
    ${FMT_LIBRARY_DIRS}
    ${LIBPNG_LIBRARY_DIRS}
)

target_link_libraries(
    ${HEADLESS_NAME}
    litehtml
    ${HARFBUZZ_LIBRARIES}
    ${FONTCONFIG_LIBRARIES}
    ${JPEG_LIBRARIES}
    ${LIBPNG_LIBRARIES}
    ${FMT_LIBRARIES}
    orion
    ${requiredlibs}
)

add_custom_command(
    TARGET ${HEADLESS_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts/Roboto-Regular.ttf
    ${CMAKE_BINARY_DIR}/Roboto-Regular.ttf)

# JSON
if (ENABLE_JSON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_JSON)
    target_include_directories(${PROJECT_NAME} PRIVATE ../../third_party/json/include)
endif()

# UTF8-CPP
target_include_directories(${PROJECT_NAME} PRIVATE ../../third_party/utf8cpp/include)
