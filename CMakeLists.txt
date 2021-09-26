cmake_minimum_required(VERSION 3.11)

project(litehtml LANGUAGES C CXX)
include(CTest)
enable_testing()

# Soname
# MAJOR is incremented when symbols are removed or changed in an incompatible way
# MINOR is incremented when new symbols are added
set(PROJECT_MAJOR 0)
set(PROJECT_MINOR 0)

option(EXTERNAL_GUMBO "Link against external gumbo instead of shipping a bundled copy" OFF)
if(WIN32)
  option(EXTERNAL_XXD "Use external xxd" OFF)
else()
  option(EXTERNAL_XXD "Use external xxd" ON)
endif()

if(NOT EXTERNAL_GUMBO)
add_subdirectory(src/gumbo)
endif()

set(SOURCE_LITEHTML
    src/background.cpp
    src/box.cpp
    src/codepoint.cpp
    src/context.cpp
    src/css_block.cpp
    src/css_component_value.cpp
    src/css_declaration.cpp
    src/css_function.cpp
    src/css_length.cpp
    src/css_number.cpp
    src/css_parser.cpp
    src/css_prelude.cpp
    src/css_rule.cpp
    src/css_selector.cpp
    src/css_stylesheet.cpp
    src/css_token.cpp
    src/css_token_range.cpp
    src/css_tokenizer.cpp
    src/css_tokenizer_input_stream.cpp
    src/css_value.cpp
    src/document.cpp
    src/document_container.cpp
    src/el_anchor.cpp
    src/el_base.cpp
    src/el_before_after.cpp
    src/el_body.cpp
    src/el_break.cpp
    src/el_cdata.cpp
    src/el_comment.cpp
    src/el_div.cpp
    src/element.cpp
    src/el_font.cpp
    src/el_image.cpp
    src/el_link.cpp
    src/el_li.cpp
    src/el_para.cpp
    src/el_script.cpp
    src/el_space.cpp
    src/el_style.cpp
    src/el_table.cpp
    src/el_td.cpp
    src/el_text.cpp
    src/el_title.cpp
    src/el_tr.cpp
    src/html.cpp
    src/html_tag.cpp
    src/iterators.cpp
    src/list_marker.cpp
    src/media_query.cpp
    src/style.cpp
    src/table.cpp
    src/tstring_view.cpp
    src/url.cpp
    src/url_path.cpp
    src/utf8_strings.cpp
    src/web_color.cpp
    src/num_cvt.cpp
)

set(HEADER_LITEHTML
    include/litehtml.h
    include/litehtml/attributes.h
    include/litehtml/background.h
    include/litehtml/borders.h
    include/litehtml/box.h
    include/litehtml/codepoint.h
    include/litehtml/context.h
    include/litehtml/css_block.h
    include/litehtml/css_component_value.h
    include/litehtml/css_declaration.h
    include/litehtml/css_function.h
    include/litehtml/css_length.h
    include/litehtml/css_margins.h
    include/litehtml/css_number.h
    include/litehtml/css_offsets.h
    include/litehtml/css_parser.h
    include/litehtml/css_position.h
    include/litehtml/css_prelude.h
    include/litehtml/css_rule.h
    include/litehtml/css_selector.h
    include/litehtml/css_stylesheet.h
    include/litehtml/css_token.h
    include/litehtml/css_token_range.h
    include/litehtml/css_tokenizer.h
    include/litehtml/css_tokenizer_input_stream.h
    include/litehtml/css_value.h
    include/litehtml/document.h
    include/litehtml/document_container.h
    include/litehtml/el_anchor.h
    include/litehtml/el_base.h
    include/litehtml/el_before_after.h
    include/litehtml/el_body.h
    include/litehtml/el_break.h
    include/litehtml/el_cdata.h
    include/litehtml/el_comment.h
    include/litehtml/el_div.h
    include/litehtml/el_font.h
    include/litehtml/el_image.h
    include/litehtml/el_link.h
    include/litehtml/el_para.h
    include/litehtml/el_script.h
    include/litehtml/el_space.h
    include/litehtml/el_style.h
    include/litehtml/el_table.h
    include/litehtml/el_td.h
    include/litehtml/el_text.h
    include/litehtml/el_title.h
    include/litehtml/el_tr.h
    include/litehtml/element.h
    include/litehtml/html.h
    include/litehtml/html_tag.h
    include/litehtml/iterators.h
    include/litehtml/list_marker.h
    include/litehtml/media_query.h
    include/litehtml/os_types.h
    include/litehtml/style.h
    include/litehtml/table.h
    include/litehtml/tstring_view.h
    include/litehtml/types.h
    include/litehtml/url.h
    include/litehtml/url_path.h
    include/litehtml/utf8_strings.h
    include/litehtml/web_color.h
    include/litehtml/num_cvt.h
)

set(SOURCE_HEADLESS
    src/headless/headless.cpp
    src/headless/headless_container.cpp
)

set(TEST_LITEHTML
    containers/test/container_test.cpp
    test/codepoint_test.cpp
    test/contextTest.cpp
    test/cssTest.cpp
    test/css_parser_test.cpp
    test/css_tokenizer_input_stream_test.cpp
    test/css_tokenizer_test.cpp
    test/documentTest.cpp
    test/layoutGlobalTest.cpp
    test/mediaQueryTest.cpp
    test/tstring_view_test.cpp
    test/url_test.cpp
    test/url_path_test.cpp
    test/webColorTest.cpp
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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_include_directories(${PROJECT_NAME} PRIVATE include/${PROJECT_NAME})

option(LITEHTML_UTF8 "Build litehtml with UTF-8 text conversion functions." OFF)
if (LITEHTML_UTF8)
  target_compile_definitions(${PROJECT_NAME} PUBLIC LITEHTML_UTF8)
endif()

# ICU
option(USE_ICU "Use ICU to locate word boundaries in text elements" OFF)
if (USE_ICU)
    target_compile_definitions(${PROJECT_NAME} PUBLIC USE_ICU)
    target_link_libraries(${PROJECT_NAME} PUBLIC icui18n icuuc icudata)
endif()

# Gumbo
target_link_libraries(${PROJECT_NAME} PUBLIC gumbo)

# JSON
option(ENABLE_JSON "Enable internal data structure serialization to JSON" OFF)
if (ENABLE_JSON)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_JSON)
    target_include_directories(${PROJECT_NAME} PRIVATE third_party/json/include)
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

# Binary Master.css
if (NOT EXTERNAL_XXD)
    add_executable(xxd xxd/xxd.c)
    set(XXD_COMMAND $<TARGET_FILE:xxd>)
else()
    find_program(XXD_COMMAND xxd)
endif()
if(WIN32)
    file(TO_NATIVE_PATH ${XXD_COMMAND} XXD_COMMAND)
    file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include/master.css MASTER_FILE)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc
        COMMAND type ${MASTER_FILE} | "${XXD_COMMAND}" -i > ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc)
else()
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc
        COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/include/master.css | xxd -i > ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc)
endif()
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc PROPERTIES GENERATED TRUE)

# Headless
option(BUILD_HEADLESS "Build headless litehtml browser" OFF)
if (BUILD_HEADLESS)
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
endif()

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc
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
        target_include_directories(${TEST_NAME} PRIVATE third_party/json/include)
    endif()

    target_link_libraries(
        ${TEST_NAME}
        ${PROJECT_NAME}
        gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(${TEST_NAME})
endif()

#     set(TEST_NAME ${PROJECT_NAME}_tests)
#     add_executable(${TEST_NAME} ${TEST_LITEHTML} ${CMAKE_CURRENT_SOURCE_DIR}/src/master.css.inc)
#     set_target_properties(${TEST_NAME} PROPERTIES
#         CXX_STANDARD 11
#         C_STANDARD 99
#         PUBLIC_HEADER "${HEADER_LITEHTML}"
#     )
#     target_include_directories(${TEST_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/containers)
#     target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME})
#     # tests
#     add_test(NAME contextTest COMMAND ${TEST_NAME} 1)
#     add_test(NAME cssTest COMMAND ${TEST_NAME} 2)
#     add_test(NAME documentTest COMMAND ${TEST_NAME} 3)
#     add_test(NAME layoutGlobalTest COMMAND ${TEST_NAME} 4)
#     add_test(NAME mediaQueryTest COMMAND ${TEST_NAME} 5)
#     add_test(NAME webColorTest COMMAND ${TEST_NAME} 6)
# endif()
