#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>
#include <libsearcher/searcher.hpp>

#include <CLI/CLI.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    try {
        CLI::App app("fts");

        std::string config_file("config.json");
        app.add_option("--config", config_file, "config");
        std::string query;
        app.add_option("--query", query, "query")->required();

        CLI11_PARSE(app, argc, argv);

        auto conf_args = fts::parser::parse_config(config_file);

        fts::index_accessor::TextIndexAccessor t_accs("./", conf_args);

        // auto info = t_accs.get_term_infos("hello");

        auto relv = fts::searcher::search(query, t_accs);

        relv.resize(11);

        for (const auto& [doc_id, rel] : relv) {
            auto str = t_accs.load_document(doc_id);
            std::cout << doc_id << ' ' << rel << ' ' << str << '\n';
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
