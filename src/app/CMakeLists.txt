set(target_name fts)

add_executable(${target_name})

include(CompileOptions)
set_compile_options(${target_name})

target_sources(
    ${target_name}
    PRIVATE
    app/main.cpp
    app/bookindexer.cpp
    app/booksearcher.cpp
)

target_include_directories(
    ${target_name}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${target_name}
    PRIVATE
    CLI11
    replxx
    parser
    index
    searcher
    index_accessor
)

install(TARGETS ${target_name} DESTINATION bin)
