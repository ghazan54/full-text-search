#pragma once

#include <libparser/parser.hpp>

#include <filesystem>

#include <map>
#include <string>

namespace fts::index {

using ForwardIndex = std::map<size_t, std::string>;
using ReverseIndex = std::map<std::string, std::map<size_t, std::set<size_t>>>;

using fspath = std::filesystem::path;

struct Index {
    ForwardIndex docs_;
    ReverseIndex entries_;
};

class IndexWriter {
   public:
    virtual void write(const fspath& path, const Index& index) = 0;
    virtual ~IndexWriter() = default;
};

class TextIndexWriter final : public IndexWriter {
   public:
    void write(const fspath& path, const Index& index) final;
    static std::string name_to_hash(const std::string& name);

   private:
    static bool write_forward_index(const fspath& path,
                                    const ForwardIndex& forward_index);
    static bool write_reverse_index(const fspath& path,
                                    const ReverseIndex& reverse_index);
    static std::string reverse_index_info_to_str(
        const std::string& term,
        const std::map<size_t, std::set<size_t>>& idx_info);
};

class IndexBuilder final {
   public:
    explicit IndexBuilder(parser::ConfArgs args) : conf_(std::move(args)) {}
    void add_document(size_t document_id, const std::string& text);
    Index index() const;

   private:
    Index index_;
    parser::ConfArgs conf_;
};

}  // namespace fts::index
