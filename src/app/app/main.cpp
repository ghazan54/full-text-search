#include <app/app.hpp>

#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>
#include <libsearcher/searcher.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    std::string cur_subcommand;
    try {
        CLI::App app("fts");

        auto indexer = indexer_init(app);
        auto searcher = searcher_init(app);

        app.require_subcommand();

        CLI11_PARSE(app, argc, argv);

        if (indexer.indexer->parsed()) {
            cur_subcommand = "indexer: ";
            indexer_parse_and_write(indexer);
        } else if (searcher.searcher->parsed()) {
            cur_subcommand = "searcher: ";
            searcher_search_and_print(searcher);
        } else {
            std::cerr << "Use subcommand 'indexer' or 'searcher'.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "fts: " << cur_subcommand << e.what() << '\n';
    }
}
