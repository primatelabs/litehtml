cmake_minimum_required(VERSION 3.11)

project(litehtml LANGUAGES C CXX)
include(CTest)
enable_testing()

option(ENABLE_JSON "Enable internal data structure serialization to JSON" OFF)
option(USE_ICU "Use ICU to locate word boundaries in text elements" OFF)

# Soname
# MAJOR is incremented when symbols are removed or changed in an incompatible way
# MINOR is incremented when new symbols are added
set(PROJECT_MAJOR 0)
set(PROJECT_MINOR 0)

set(SOURCE_LITEHTML
    background.cpp
    background_paint.cpp
    box.cpp
    codepoint.cpp
    color.cpp
    context.cpp
    css/css_block.cpp
    css/css_component_value.cpp
    css/css_declaration.cpp
    css/css_function.cpp
    css/css_length.cpp
    css/css_number.cpp
    css/css_parser.cpp
    css/css_prelude.cpp
    css/css_property.cpp
    css/css_range.cpp
    css/css_regenerate.cpp
    css/css_rule.cpp
    css/css_selector.cpp
    css/css_style.cpp
    css/css_stylesheet.cpp
    css/css_token.cpp
    css/css_tokenizer.cpp
    css/css_tokenizer_input_stream.cpp
    css/css_value.cpp
    document.cpp
    document_container.cpp
    document_parser.cpp
    element/anchor_element.cpp
    element/base_element.cpp
    element/before_after_element.cpp
    element/body_element.cpp
    element/break_element.cpp
    element/cdata_element.cpp
    element/comment_element.cpp
    element/div_element.cpp
    element/element.cpp
    element/font_element.cpp
    element/html_element.cpp
    element/image_element.cpp
    element/li_element.cpp
    element/link_element.cpp
    element/paragraph_element.cpp
    element/script_element.cpp
    element/whitespace_element.cpp
    element/style_element.cpp
    element/table_element.cpp
    element/td_element.cpp
    element/text_element.cpp
    element/title_element.cpp
    element/tr_element.cpp
    html.cpp
    iterators.cpp
    list_marker.cpp
    logging.cpp
    media_features.cpp
    media_query.cpp
    media_query_expression.cpp
    media_query_list.cpp
    num_cvt.cpp
    string_view.cpp
    table.cpp
    text.cpp
    url.cpp
    url_path.cpp
    utf8_strings.cpp
)

set(HEADER_LITEHTML
    include/litehtml/background.h
    include/litehtml/background_paint.h
    include/litehtml/borders.h
    include/litehtml/box.h
    include/litehtml/codepoint.h
    include/litehtml/color.h
    include/litehtml/context.h
    include/litehtml/css/css_block.h
    include/litehtml/css/css_component_value.h
    include/litehtml/css/css_declaration.h
    include/litehtml/css/css_function.h
    include/litehtml/css/css_length.h
    include/litehtml/css/css_margins.h
    include/litehtml/css/css_number.h
    include/litehtml/css/css_offsets.h
    include/litehtml/css/css_parser.h
    include/litehtml/css/css_position.h
    include/litehtml/css/css_prelude.h
    include/litehtml/css/css_property.h
    include/litehtml/css/css_range.h
    include/litehtml/css/css_rule.h
    include/litehtml/css/css_selector.h
    include/litehtml/css/css_style.h
    include/litehtml/css/css_stylesheet.h
    include/litehtml/css/css_token.h
    include/litehtml/css/css_tokenizer.h
    include/litehtml/css/css_tokenizer_input_stream.h
    include/litehtml/css/css_value.h
    include/litehtml/document.h
    include/litehtml/document_container.h
    include/litehtml/element/anchor_element.h
    include/litehtml/element/base_element.h
    include/litehtml/element/before_after_element.h
    include/litehtml/element/body_element.h
    include/litehtml/element/break_element.h
    include/litehtml/element/cdata_element.h
    include/litehtml/element/comment_element.h
    include/litehtml/element/div_element.h
    include/litehtml/element/element.h
    include/litehtml/element/font_element.h
    include/litehtml/element/html_element.h
    include/litehtml/element/image_element.h
    include/litehtml/element/link_element.h
    include/litehtml/element/paragraph_element.h
    include/litehtml/element/script_element.h
    include/litehtml/element/whitespace_element.h
    include/litehtml/element/style_element.h
    include/litehtml/element/table_element.h
    include/litehtml/element/td_element.h
    include/litehtml/element/text_element.h
    include/litehtml/element/title_element.h
    include/litehtml/element/tr_element.h
    include/litehtml/html.h
    include/litehtml/iterators.h
    include/litehtml/list_marker.h
    include/litehtml/litehtml.h
    include/litehtml/logging.h
    include/litehtml/media_features.h
    include/litehtml/media_query.h
    include/litehtml/media_query_expression.h
    include/litehtml/media_query_list.h
    include/litehtml/num_cvt.h
    include/litehtml/string.h
    include/litehtml/string_view.h
    include/litehtml/table.h
    include/litehtml/text.h
    include/litehtml/types.h
    include/litehtml/url.h
    include/litehtml/url_path.h
    include/litehtml/utf8_strings.h
)

set(TEST_LITEHTML
    codepoint_test.cpp
    color_test.cpp
    context_test.cpp
    css/css_length_test.cpp
    css/css_parser_test.cpp
    css/css_regenerate_test.cpp
    css/css_selector_test.cpp
    css/css_stylesheet_test.cpp
    css/css_test.cpp
    css/css_tokenizer_input_stream_test.cpp
    css/css_tokenizer_test.cpp
    document_parser_test.cpp
    document_test.cpp
    layout_global_test.cpp
    media_query_expression_test.cpp
    media_query_test.cpp
    string_view_test.cpp
    text_test.cpp
    url_path_test.cpp
    url_test.cpp

    test_container.cpp
)

set(PROJECT_LIB_VERSION ${PROJECT_MAJOR}.${PROJECT_MINOR}.0)
set(PROJECT_SO_VERSION ${PROJECT_MAJOR})

add_library(${PROJECT_NAME} ${SOURCE_LITEHTML})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_LIB_VERSION}
    SOVERSION ${PROJECT_SO_VERSION}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 14
    C_STANDARD 99
    PUBLIC_HEADER "${HEADER_LITEHTML}"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Werror>
)

# Export litehtml includes.

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# ICU
if (USE_ICU)
    target_compile_definitions(${PROJECT_NAME} PUBLIC USE_ICU)
    target_link_libraries(${PROJECT_NAME} PUBLIC icui18n icuuc icudata)
endif()

# Gumbo
target_link_libraries(${PROJECT_NAME} PUBLIC gumbo)

# JSON
if (ENABLE_JSON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_JSON)
    target_include_directories(${PROJECT_NAME} PRIVATE ../../third_party/json/include)
endif()

# UTF8CPP
target_include_directories(${PROJECT_NAME} PRIVATE ../../third_party/utf8cpp/include)

# install and export
install(TARGETS ${PROJECT_NAME}
    EXPORT litehtmlTargets
    RUNTIME DESTINATION bin COMPONENT libraries
    ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT libraries
    LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT libraries
    PUBLIC_HEADER DESTINATION include/litehtml
)
install(FILES cmake/litehtmlConfig.cmake DESTINATION lib${LIB_SUFFIX}/cmake/litehtml)
install(EXPORT litehtmlTargets FILE litehtmlTargets.cmake DESTINATION lib${LIB_SUFFIX}/cmake/litehtml)

# Binary master.css
find_package(Python3 REQUIRED)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc"
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/process-css.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/master.css"
        "${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/master.css")

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc PROPERTIES GENERATED TRUE)


# Generate CSS property name files

add_custom_command(
    OUTPUT
        "${CMAKE_CURRENT_SOURCE_DIR}/css/css_property.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/litehtml/css/css_property.h"
    COMMAND
        "${Python3_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/generate_css_property.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/css_property.json"
        "${CMAKE_CURRENT_SOURCE_DIR}/css/css_property.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/litehtml/css/css_property.h"
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/css_property.json"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/css_property.cpp.mustache"
        "${CMAKE_CURRENT_SOURCE_DIR}/scripts/css_property.h.mustache")

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/css/css_property.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/include/litehtml/css/css_property.h PROPERTIES GENERATED TRUE)


# Tests

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

if (BUILD_TESTING)
    enable_testing()

    set(TEST_NAME ${PROJECT_NAME}_tests)

    add_executable(
        ${TEST_NAME}
        ${TEST_LITEHTML}
        ${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc
    )

    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 14
        C_STANDARD 99
        PUBLIC_HEADER "${HEADER_LITEHTML}"
    )

    target_include_directories(
        ${TEST_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/containers
    )

    if (ENABLE_JSON)
        target_compile_definitions(${TEST_NAME} PRIVATE ENABLE_JSON)
        target_include_directories(${TEST_NAME} PRIVATE ../../third_party/json/include)
    endif()

    target_link_libraries(
        ${TEST_NAME}
        ${PROJECT_NAME}
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${TEST_NAME})
endif()
