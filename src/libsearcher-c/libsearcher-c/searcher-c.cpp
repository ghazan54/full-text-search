#include <libsearcher-c/searcher-c.h>

#include <libsearcher/searcher.hpp>

#include <iostream>

IndexAccessor* fts_create_handle() {
    fts::parser::ConfArgs conf;
    conf.ngram_min_length_ = 3;
    conf.ngram_max_length_ = 6;
    conf.stop_words_ = {
        "a",     "an",    "and",  "are",  "as", "at",   "be",   "but",   "by",
        "for",   "if",    "in",   "into", "is", "it",   "no",   "not",   "of",
        "on",    "or",    "s",    "such", "t",  "that", "the",  "their", "then",
        "there", "these", "they", "this", "to", "was",  "will", "with"};

    return reinterpret_cast<IndexAccessor*>(
        static_cast<fts::index_accessor::IndexAccessor*>(
            new fts::index_accessor::TextIndexAccessor(".", conf)));
}

void fts_delete_handle(IndexAccessor* accessor) {
    delete reinterpret_cast<fts::index_accessor::IndexAccessor*>(accessor);
}

Results* fts_search(const char* query, const IndexAccessor* accessor) {
    auto t_accs = *(dynamic_cast<const fts::index_accessor::TextIndexAccessor*>(
        reinterpret_cast<const fts::index_accessor::IndexAccessor*>(accessor)));

    const auto result = fts::searcher::search(query, t_accs);

    return reinterpret_cast<Results*>(new fts::searcher::Results(result));
}

void fts_print_result(const Results* res, const IndexAccessor* accessor) {
    const auto result = *reinterpret_cast<const fts::searcher::Results*>(res);
    auto t_accs = *(dynamic_cast<const fts::index_accessor::TextIndexAccessor*>(
        reinterpret_cast<const fts::index_accessor::IndexAccessor*>(accessor)));

    for (const auto& [doc_id, rel] : result) {
        auto str = t_accs.load_document(doc_id);
        std::cout << doc_id << ' ' << rel << ' ' << str << '\n';
    }
}