#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <algorithm>
#include <cstring>
#include <iostream>

namespace fts::index {

void IndexBuilder::add_document(size_t document_id, const std::string& text) {
    index_.docs_.insert(std::make_pair(document_id, text));

    auto ngrams = parser::parse_ngram(text, conf_);

    for (size_t i = 0; i < ngrams.size(); ++i) {
        for (const auto& word : ngrams[i]) {
            index_.entries_[word].insert(std::make_pair(document_id, i));
        }
    }
}

Index IndexBuilder::index() { return index_; }

}  // namespace fts::index
