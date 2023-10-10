#pragma once

#include <CLI/CLI.hpp>

struct Indexer {
    std::string config_path = "config.json";
    std::string csv_path;
    std::string index_path;
    CLI::App* indexer;
};

struct Searcher {
    std::string config_path = "config.json";
    std::string index_path;
    std::string query;
    CLI::App* searcher;
};

Indexer indexer_init(CLI::App& app);
void indexer_parse_and_write(const Indexer& indexer);

Searcher searcher_init(CLI::App& app);
void searcher_search_and_print(const Searcher& searcher);
