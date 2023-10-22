#include <libsearcher-c/searcher-c.h>

#include <libsearcher/searcher.hpp>

#include <iostream>

#include <algorithm>
#include <cstring>

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

void fts_delete_results(Results* res) {
    delete reinterpret_cast<fts::searcher::Results*>(res);
}

CResult fts_get_row_info(const Results* res, const size_t idx,
                         const IndexAccessor* accessor) {
    CResult cres;
    const auto results = *reinterpret_cast<const fts::searcher::Results*>(res);
    auto t_accs = *(dynamic_cast<const fts::index_accessor::TextIndexAccessor*>(
        reinterpret_cast<const fts::index_accessor::IndexAccessor*>(accessor)));

    const auto str = t_accs.load_document(results[idx].doc_id);
    cres.str = new char[str.size() + 1];
    // std::copy(str.begin(), str.end(), cres.str);
    strncpy(cres.str, str.c_str(), str.size() + 1);
    cres.doc_id = results[idx].doc_id;
    cres.score = results[idx].score;
    return cres;
}

size_t fts_get_results_size(const Results* res) {
    const auto results = *reinterpret_cast<const fts::searcher::Results*>(res);
    return results.size();
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
