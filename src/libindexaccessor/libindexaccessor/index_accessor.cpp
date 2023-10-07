#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>

namespace fts::index_accessor {

index::ReverseIndex TextIndexAccessor::get_term_infos(const std::string& term) {
    const std::string path(path_ / "index/entries" /
                           index::TextIndexWriter::name_to_hash(term));
    std::ifstream entry_file(path);
    index::ReverseIndex term_info;

    if (!entry_file.is_open()) {
        return term_info;
    }

    std::string line;
    while (std::getline(entry_file, line)) {
        std::istringstream entry(line);

        {
            std::string word;
            entry >> word;
            if (term != word) {
                continue;
            }
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
                term_info[term][doc_id].push_back(pos);
            }
        }
    }

    return term_info;
}

std::string TextIndexAccessor::load_document(size_t document_id) {
    const std::string path(path_ / "index" / "docs" /
                           std::to_string(document_id));
    std::ifstream document(path);

    if (!document.is_open()) {
        throw std::ios_base::failure("Could not open the file " + path);
    }

    std::string text;

    std::getline(document, text);

    document.close();

    return text;
}

size_t TextIndexAccessor::total_docs() const {
    size_t total = 0;
    const std::string path(path_ / "index" / "total");
    std::ifstream ttl(path);

    if (!ttl.is_open()) {
        throw std::ios_base::failure("Could not open the file " + path);
    }

    ttl >> total;

    return total;
}

parser::ConfArgs TextIndexAccessor::config() const { return conf_; }

}  // namespace fts::index_accessor
