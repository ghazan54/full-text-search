#pragma once

#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <filesystem>

#include <map>
#include <set>
#include <string>

namespace fts::index_accessor {

using fspath = std::filesystem::path;

class IndexAccessor {
   public:
    virtual ~IndexAccessor() = default;

    virtual index::ReverseIndex get_term_infos(const std::string& term) = 0;
    virtual std::string load_document(size_t document_id) = 0;
    virtual size_t total_docs() = 0;
    virtual parser::ConfArgs config() = 0;
};

class TextIndexAccessor final : public IndexAccessor {
   public:
    TextIndexAccessor() = delete;
    TextIndexAccessor(fspath path, parser::ConfArgs conf)
        : path_(std::move(path)), conf_(std::move(conf)) {}
    ~TextIndexAccessor() override = default;

    index::ReverseIndex get_term_infos(const std::string& term) override;
    std::string load_document(size_t document_id) override;
    size_t total_docs() override;
    parser::ConfArgs config() override;

   private:
    fspath path_;
    parser::ConfArgs conf_;
};

}  // namespace fts::index_accessor
