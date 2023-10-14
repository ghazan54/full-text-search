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

# set(target_name_c fts_c)

# add_executable(${target_name_c} app/main.c)

# include(CompileOptions)

# set_compile_options(${target_name_c})
# set_target_properties(
# ${target_name_c}
# PROPERTIES
# C_STANDARD 17
# C_STANDARD_REQUIRED ON
# C_EXTENSIONS OFF
# )

# target_include_directories(
# ${target_name_c}
# PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}
# )

# target_link_libraries(
# fts_c
# PUBLIC
# searcher-c
# )
