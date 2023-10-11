#include <app/app.hpp>

#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <iostream>

Indexer indexer_init(CLI::App& app) {
    Indexer indexer;

    indexer.indexer = app.add_subcommand("indexer", "Index documents");

    indexer.indexer->add_option("--config", indexer.config_path,
                                "The path to the config");

    indexer.indexer
        ->add_option("--csv", indexer.csv_path, "The path to the .csv file")
        ->required();

    indexer.indexer
        ->add_option("--index", indexer.index_path,
                     "Where will the index directory be located")
        ->required();

    return indexer;
}

void indexer_parse_and_write(const Indexer& indexer) {
    auto config = fts::parser::parse_config(indexer.config_path);
    auto books_info = fts::parser::parse_csv(indexer.csv_path);
    fts::index::IndexBuilder builder(config);

    for (const auto& row : books_info) {
        builder.add_document(row.book_id, row.title);
    }
    auto index = builder.index();

    const fts::index::TextIndexWriter writer;
    writer.write(indexer.index_path, index);
}
