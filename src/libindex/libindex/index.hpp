#pragma once

#include <libparser/parser.hpp>

#include <filesystem>

#include <map>
#include <string>

namespace fts::index {

using ForwardIndex = std::map<size_t, std::string>;
using ReverseIndex = std::map<std::string, std::multimap<size_t, size_t>>;

using fspath = std::filesystem::path;

struct Index {
    ForwardIndex docs_;
    ReverseIndex entries_;
};

class IndexWriter {
   public:
    virtual Index write(const fspath& path) = 0;
    virtual ~IndexWriter() = default;
};

class TextIndexWriter final : public IndexWriter {
   public:
    Index write(const fspath& path) final;
    ~TextIndexWriter() final = default;
};

class IndexBuilder final {
   public:
    IndexBuilder() = delete;
    explicit IndexBuilder(parser::ConfArgs args) : conf_(std::move(args)) {}
    void add_document(size_t document_id, const std::string& text);
    Index index();

   private:
    Index index_;
    parser::ConfArgs conf_;
};

}  // namespace fts::index
