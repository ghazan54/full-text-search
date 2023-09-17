#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <picosha2.h>

#include <map>
#include <set>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

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

namespace {

void create_dir(const fspath& path) {
    if (!std::filesystem::exists(path)) {
        if (!std::filesystem::create_directory(path)) {
            std::cerr << "fts: index: error create dir: " << path << '\n';
        }
    }
}

}  // namespace

void TextIndexWriter::write(const fspath& path, const Index& index) {
    const fspath index_path = path / "index";
    const fspath forward_index_path = index_path / "docs";
    const fspath reverse_index_path = index_path / "entries";

    create_dir(index_path);
    create_dir(forward_index_path);
    create_dir(reverse_index_path);

    if (!write_forward_index(forward_index_path, index.docs_)) {
        std::cerr << "fts: index: error print forward index." << '\n';
    }

    if (!write_reverse_index(reverse_index_path, index.entries_)) {
        std::cerr << "fts: index: error print reverse index." << '\n';
    }
}

bool TextIndexWriter::write_forward_index(const fspath& path,
                                          const ForwardIndex& forward_index) {
    for (const auto& [doc_id, text] : forward_index) {
        std::ofstream out_file(path / std::to_string(doc_id));

        if (!out_file.is_open()) {
            return false;
        }

        out_file << text;

        out_file.close();
    }

    return true;
}

bool TextIndexWriter::write_reverse_index(const fspath& path,
                                          const ReverseIndex& reverse_index) {
    for (const auto& [term, info] : reverse_index) {
        std::ofstream out_file(path / name_to_hash(term));
        if (!out_file.is_open()) {
            return false;
        }

        out_file << reverse_index_info_to_str(term, info);

        out_file.close();
    }

    return true;
}

std::string TextIndexWriter::name_to_hash(const std::string& name) {
    return picosha2::hash256_hex_string(name).substr(0, 6);
}

std::string TextIndexWriter::reverse_index_info_to_str(
    const std::string& term, const std::multimap<size_t, size_t>& idx_info) {
    using DocIdToSetIdxs = std::map<size_t, std::set<size_t>>;

    DocIdToSetIdxs info;
    for (const auto& [doc_id, pos] : idx_info) {
        info[doc_id].insert(pos);
    }

    std::ostringstream reverse_index_str;

    reverse_index_str << term << ' ' << info.size() << ' ';
    for (const auto& [doc_id, idxs] : info) {
        reverse_index_str << doc_id << ' ' << idxs.size() << ' ';
        for (const auto& idx : idxs) {
            reverse_index_str << idx << ' ';
        }
    }

    return reverse_index_str.str();
}

}  // namespace fts::index
