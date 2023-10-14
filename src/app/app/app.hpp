#pragma once

#include <CLI/CLI.hpp>

struct IndexerOptions {
    std::string config_path = "config.json";
    std::string csv_path;
    std::string index_path;
    CLI::App* indexer;
};

struct SearcherOptions {
    std::string config_path = "config.json";
    std::string index_path;
    std::string query;
    CLI::App* searcher;
};

IndexerOptions indexer_init(CLI::App& app);
void indexer_parse_and_write(const IndexerOptions& indexer);

SearcherOptions searcher_init(CLI::App& app);
void searcher_search_and_print(const SearcherOptions& searcher);
