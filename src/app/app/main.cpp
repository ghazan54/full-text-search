#include <libparser/parser.hpp>

#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        CLI::App app("fts");

        std::string config_file("config.json");
        app.add_option("--config", config_file, "First count");

        CLI11_PARSE(app, argc, argv);

        auto conf_args = fts::parser::parse_config(config_file);
        auto ret =
            fts::parser::parse_ngram("Dr. Jekyll and Mr. Hyde", conf_args);

        for (size_t i = 0; i < ret.size(); ++i) {
            for (size_t j = 0; j < ret[i].size(); ++j) {
                std::cout << ret[i][j] << ' ' << i << ' ';
            }
        }
        std::cout << '\n';
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
