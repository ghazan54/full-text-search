set(target_name index_accessor)

add_library(${target_name} STATIC
    libindexaccessor/index_accessor.cpp
    libindexaccessor/index_accessor.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
    ${target_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${target_name}
    PUBLIC
    parser
    index
)

# tests
include(GoogleTest)

add_executable(
    index_accessor.test
    libindexaccessor/index_accessor.test.cpp
)

target_link_libraries(
    index_accessor.test
    GTest::gtest_main
    index_accessor
)

target_include_directories(
    index_accessor.test
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

gtest_discover_tests(index_accessor.test)
