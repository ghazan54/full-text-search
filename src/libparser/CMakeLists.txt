set(target_name parser)

add_library(${target_name} STATIC
    libparser/parser.cpp
    libparser/parser.hpp)

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
    nlohmann_json::nlohmann_json
)

create_test(parser.test libparser/parser.test.cpp parser nlohmann_json::nlohmann_json)
