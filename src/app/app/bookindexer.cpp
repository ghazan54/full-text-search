#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <CLI/CLI.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        CLI::App app("fts");

        std::string config_file("config.json");
        app.add_option("--config", config_file, "First count");

        CLI11_PARSE(app, argc, argv);

        auto conf_args = fts::parser::parse_config(config_file);

        fts::index::IndexBuilder builder(conf_args);
        std::string text;
        size_t doc_id = 0;
        while ((std::cin >> doc_id >> std::ws) &&
               std::getline(std::cin, text)) {
            builder.add_document(doc_id, text);
        }
        auto index = builder.index();
        // auto reverse_index = index.entries_;

        // for (const auto& [term, info] : reverse_index) {
        //     std::cout << term << " { ";
        //     for (const auto& [doc_id, set_pos] : info) {
        //         for (const auto& pos : set_pos) {
        //             std::cout << doc_id << ": [" << pos << "], ";
        //         }
        //     }
        //     std::cout << " }\n";
        // }

        const fts::index::TextIndexWriter writer;
        writer.write("./", index);

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}