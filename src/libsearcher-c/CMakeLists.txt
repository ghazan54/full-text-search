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

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/client/fts
    COMMAND make build include_path=${CMAKE_CURRENT_LIST_DIR} slib_path=$<TARGET_FILE_DIR:${target_name}> libname=${target_name}
    DEPENDS ${CMAKE_SOURCE_DIR}/client/cmd/main.go ${CMAKE_SOURCE_DIR}/client/searcher/searcher.go
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/client
)

add_custom_target(
    build_fts
    ALL
    DEPENDS ${target_name} ${CMAKE_SOURCE_DIR}/client/fts
)
