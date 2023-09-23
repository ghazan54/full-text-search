set(target_name index)

add_library(${target_name} STATIC
    libindex/index.cpp
    libindex/index.hpp)

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
    picosha2
)

# tests
include(GoogleTest)

add_executable(
    index.test
    libindex/index.test.cpp
)

target_link_libraries(
    index.test
    GTest::gtest_main
    parser
    index
)

target_include_directories(
    index.test
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

gtest_discover_tests(index.test)
