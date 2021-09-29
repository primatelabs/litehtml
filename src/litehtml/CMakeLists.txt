cmake_minimum_required(VERSION 3.11)

project(litehtml LANGUAGES C CXX)
include(CTest)
enable_testing()

option(ENABLE_JSON "Enable internal data structure serialization to JSON" OFF)
option(LITEHTML_UTF8 "Build litehtml with UTF-8 text conversion functions." OFF)
option(USE_ICU "Use ICU to locate word boundaries in text elements" OFF)

# Soname
# MAJOR is incremented when symbols are removed or changed in an incompatible way
# MINOR is incremented when new symbols are added
set(PROJECT_MAJOR 0)
set(PROJECT_MINOR 0)

set(SOURCE_LITEHTML
    background.cpp
    box.cpp
    codepoint.cpp
    context.cpp
    css/css_block.cpp
    css/css_component_value.cpp
    css/css_declaration.cpp
    css/css_function.cpp
    css/css_length.cpp
    css/css_number.cpp
    css/css_parser.cpp
    css/css_prelude.cpp
    css/css_rule.cpp
    css/css_selector.cpp
    css/css_stylesheet.cpp
    css/css_token.cpp
    css/css_token_range.cpp
    css/css_tokenizer.cpp
    css/css_tokenizer_input_stream.cpp
    css/css_value.cpp
    document.cpp
    document_container.cpp
    element/el_anchor.cpp
    element/el_base.cpp
    element/el_before_after.cpp
    element/el_body.cpp
    element/el_break.cpp
    element/el_cdata.cpp
    element/el_comment.cpp
    element/el_div.cpp
    element/el_font.cpp
    element/el_image.cpp
    element/el_li.cpp
    element/el_link.cpp
    element/el_para.cpp
    element/el_script.cpp
    element/el_space.cpp
    element/el_style.cpp
    element/el_table.cpp
    element/el_td.cpp
    element/el_text.cpp
    element/el_title.cpp
    element/el_tr.cpp
    element/element.cpp
    element/html_tag.cpp
    html.cpp
    iterators.cpp
    list_marker.cpp
    media_query.cpp
    num_cvt.cpp
    style.cpp
    table.cpp
    tstring_view.cpp
    url.cpp
    url_path.cpp
    utf8_strings.cpp
    web_color.cpp
)

set(HEADER_LITEHTML
    include/litehtml/attributes.h
    include/litehtml/background.h
    include/litehtml/borders.h
    include/litehtml/box.h
    include/litehtml/codepoint.h
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
    include/litehtml/css/css_rule.h
    include/litehtml/css/css_selector.h
    include/litehtml/css/css_stylesheet.h
    include/litehtml/css/css_token.h
    include/litehtml/css/css_token_range.h
    include/litehtml/css/css_tokenizer.h
    include/litehtml/css/css_tokenizer_input_stream.h
    include/litehtml/css/css_value.h
    include/litehtml/document.h
    include/litehtml/document_container.h
    include/litehtml/element/el_anchor.h
    include/litehtml/element/el_base.h
    include/litehtml/element/el_before_after.h
    include/litehtml/element/el_body.h
    include/litehtml/element/el_break.h
    include/litehtml/element/el_cdata.h
    include/litehtml/element/el_comment.h
    include/litehtml/element/el_div.h
    include/litehtml/element/el_font.h
    include/litehtml/element/el_image.h
    include/litehtml/element/el_link.h
    include/litehtml/element/el_para.h
    include/litehtml/element/el_script.h
    include/litehtml/element/el_space.h
    include/litehtml/element/el_style.h
    include/litehtml/element/el_table.h
    include/litehtml/element/el_td.h
    include/litehtml/element/el_text.h
    include/litehtml/element/el_title.h
    include/litehtml/element/el_tr.h
    include/litehtml/element/element.h
    include/litehtml/element/html_tag.h
    include/litehtml/html.h
    include/litehtml/iterators.h
    include/litehtml/list_marker.h
    include/litehtml/litehtml.h
    include/litehtml/media_query.h
    include/litehtml/num_cvt.h
    include/litehtml/os_types.h
    include/litehtml/style.h
    include/litehtml/table.h
    include/litehtml/tstring_view.h
    include/litehtml/types.h
    include/litehtml/url.h
    include/litehtml/url_path.h
    include/litehtml/utf8_strings.h
    include/litehtml/web_color.h
)

set(TEST_LITEHTML
    codepoint_test.cpp
    context_test.cpp
    css/css_parser_test.cpp
    css/css_test.cpp
    css/css_tokenizer_input_stream_test.cpp
    css/css_tokenizer_test.cpp
    document_test.cpp
    layout_global_test.cpp
    media_query_test.cpp
    test_container.cpp
    tstring_view_test.cpp
    url_path_test.cpp
    url_test.cpp
    web_color_test.cpp
)

set(PROJECT_LIB_VERSION ${PROJECT_MAJOR}.${PROJECT_MINOR}.0)
set(PROJECT_SO_VERSION ${PROJECT_MAJOR})

add_library(${PROJECT_NAME} ${SOURCE_LITEHTML})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_LIB_VERSION} SOVERSION ${PROJECT_SO_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    C_STANDARD 99
    PUBLIC_HEADER "${HEADER_LITEHTML}"
)

# Export litehtml includes.

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

# UTF8

if (LITEHTML_UTF8)
  target_compile_definitions(${PROJECT_NAME} PUBLIC LITEHTML_UTF8)
endif()

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
find_package(PythonInterp REQUIRED)

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc"
    COMMAND "${PYTHON_EXECUTABLE}"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/process-css.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/master.css"
        "${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/master.css")

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/master.css.inc PROPERTIES GENERATED TRUE)

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
        CXX_STANDARD 11
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
