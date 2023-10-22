set(target_name searcher-c)

add_library(${target_name} SHARED
    libsearcher-c/searcher-c.cpp
    libsearcher-c/searcher-c.h)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
    ${target_name}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(
    ${target_name}
    PRIVATE
    searcher
)

add_custom_target(
    run_make_build
    COMMAND make build include_path=${CMAKE_CURRENT_LIST_DIR} slib_path=$<TARGET_FILE_DIR:${target_name}> libname=${target_name}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/client
)

add_dependencies(${target_name} run_make_build)
