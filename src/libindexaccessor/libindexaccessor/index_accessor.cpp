#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>

namespace fts::index_accessor {

index::ReverseIndex TextIndexAccessor::get_term_infos(const std::string& term) {
    std::ifstream entry(path_ / "index/entries" /
                        index::TextIndexWriter::name_to_hash(term));

    index::ReverseIndex term_info;
    {
        std::string word;
        entry >> word;
    }
    size_t count_entries = 0;
    entry >> count_entries;
    for (size_t i = 0; i < count_entries; ++i) {
        size_t doc_id = 0;
        entry >> doc_id;
        size_t count_pos = 0;
        entry >> count_pos;
        for (size_t j = 0; j < count_pos; ++j) {
            size_t pos = 0;
            entry >> pos;
            // term_info[term].insert(std::make_pair(doc_id, pos));
            term_info[term][doc_id].insert(pos);
        }
    }

    return term_info;
}

std::string TextIndexAccessor::load_document(size_t document_id) {
    std::ifstream document(path_ / "index/docs" / std::to_string(document_id));
    std::string text;

    std::getline(document, text);

    document.close();

    return text;
}

size_t TextIndexAccessor::total_docs() {
    auto dir_iter = std::filesystem::directory_iterator(path_ / "index/docs");

    size_t total = 0;
    for (const auto& entry : dir_iter) {
        if (entry.is_regular_file()) {
            ++total;
        }
    }

    return total;
}

parser::ConfArgs TextIndexAccessor::config() { return conf_; }

}  // namespace fts::index_accessor
