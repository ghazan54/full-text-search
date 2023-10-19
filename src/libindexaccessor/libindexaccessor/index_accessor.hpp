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

    virtual index::ReverseIndex get_term_infos(
        const std::string& term) const = 0;
    virtual std::string load_document(size_t document_id) const = 0;
    virtual size_t total_docs() const = 0;
    virtual parser::ConfArgs config() const = 0;
};

class TextIndexAccessor final : public IndexAccessor {
   public:
    explicit TextIndexAccessor(fspath path) : path_(std::move(path)) {
        const fspath conf_path(path_ / "index" / "config.json");
        if (!std::filesystem::exists(conf_path)) {
            throw std::ios_base::failure("Unable to find the config for index");
        }
        conf_ = parser::parse_config(conf_path);
    }

    index::ReverseIndex get_term_infos(const std::string& term) const override;
    std::string load_document(size_t document_id) const override;
    size_t total_docs() const override;
    parser::ConfArgs config() const override;

   private:
    fspath path_;
    parser::ConfArgs conf_;
};

}  // namespace fts::index_accessor
