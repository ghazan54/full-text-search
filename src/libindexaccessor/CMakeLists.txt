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

create_test(index_accessor.test libindexaccessor/index_accessor.test.cpp index_accessor)
