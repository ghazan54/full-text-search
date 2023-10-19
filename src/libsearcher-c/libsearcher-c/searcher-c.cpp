#include <libsearcher-c/searcher-c.h>

#include <libsearcher/searcher.hpp>

#include <iostream>

IndexAccessor* fts_create_handle(const char* path) {
    return reinterpret_cast<IndexAccessor*>(
        static_cast<fts::index_accessor::IndexAccessor*>(
            new fts::index_accessor::TextIndexAccessor(path)));
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
    const auto results = *reinterpret_cast<const fts::searcher::Results*>(res);
    auto t_accs = *(dynamic_cast<const fts::index_accessor::TextIndexAccessor*>(
        reinterpret_cast<const fts::index_accessor::IndexAccessor*>(accessor)));

    for (const auto& result : results) {
        auto str = t_accs.load_document(result.doc_id);
        std::cout << result.doc_id << ' ' << result.score << ' ' << str << '\n';
    }
}
