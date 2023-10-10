set(target_name bookindexer)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    app/bookindexer.cpp
)

target_link_libraries(
    ${target_name}
    PRIVATE
    CLI11
    parser
    index
)

set(target_name booksearcher)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    app/booksearcher.cpp
)

target_link_libraries(
    ${target_name}
    PRIVATE
    CLI11
    parser
    index_accessor
    searcher
)
