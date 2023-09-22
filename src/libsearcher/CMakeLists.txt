set(target_name searcher)

add_library(${target_name} STATIC
    libsearcher/searcher.cpp
    libsearcher/searcher.hpp)

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
    index_accessor
)

# tests
include(GoogleTest)

add_executable(
    searcher.test
    libsearcher/searcher.test.cpp
)

target_link_libraries(
    searcher.test
    GTest::gtest_main
    searcher
)

target_include_directories(
    searcher.test
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

gtest_discover_tests(searcher.test)
