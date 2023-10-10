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

create_test(searcher.test libsearcher/searcher.test.cpp searcher)
