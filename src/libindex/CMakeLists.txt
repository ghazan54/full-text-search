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

create_test(index.test libindex/index.test.cpp parser index)
