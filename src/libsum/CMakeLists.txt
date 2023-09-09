set(target_name sum)

add_library(${target_name} STATIC
    libsum/sum.cpp
    libsum/sum.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
    ${target_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

# tests
include(GoogleTest)

add_executable(
    sum.test
    libsum/sum.test.cpp
)

target_link_libraries(
    sum.test
    GTest::gtest_main
    sum
)

target_include_directories(
    sum.test
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

gtest_discover_tests(sum.test)
