#include <app/app.hpp>

#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>
#include <libsearcher/searcher.hpp>

#include <replxx.hxx>

#include <fstream>
#include <iostream>

Searcher searcher_init(CLI::App& app) {
    Searcher searcher;

    searcher.searcher =
        app.add_subcommand("searcher", "Search among indexed documents");

    searcher.searcher
        ->add_option("--index", searcher.index_path,
                     "Where will the index directory be taken from")
        ->required();
    searcher.searcher->add_option("--config", searcher.config_path,
                                  "The path to the config");
    searcher.searcher->add_option("--query", searcher.query,
                                  "Query for searcher");

    return searcher;
}

namespace {

void searcher_print(const fts::searcher::Results& result,
                    const fts::index_accessor::IndexAccessor& accessor) {
    for (const auto& [doc_id, rel] : result) {
        auto str = accessor.load_document(doc_id);
        std::cout << doc_id << ' ' << rel << ' ' << str << '\n';
    }
}

}  // namespace

void searcher_search_and_print(const Searcher& searcher) {
    auto conf_args = fts::parser::parse_config(searcher.config_path);

    fts::index_accessor::TextIndexAccessor accessor(searcher.index_path,
                                                    conf_args);

    if (searcher.query.empty()) {
        const fts::parser::fspath temp_dir =
            std::filesystem::temp_directory_path();
        const fts::parser::fspath hist_path = temp_dir / "replxx.hist";
        replxx::Replxx repl;
        repl.history_save(hist_path);

        while (true) {
            const std::string query = repl.input("> ");
            repl.clear_screen();
            if (query.empty()) {
                break;
            }
            auto result = fts::searcher::search(query, accessor);
            searcher_print(result, accessor);
            repl.history_add(query);
        }
    } else {
        auto result = fts::searcher::search(searcher.query, accessor);
        searcher_print(result, accessor);
    }
}
