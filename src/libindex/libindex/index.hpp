#pragma once

#include <libparser/parser.hpp>

#include <filesystem>

#include <map>
#include <string>

namespace fts::index {

/**
 * @brief Alias for a map representing the forward index.
 */
using ForwardIndex = std::map<size_t, std::string>;

/**
 * @brief Alias for a map representing the reverse index.
 */
using ReverseIndex =
    std::map<std::string, std::map<size_t, std::vector<size_t>>>;

/**
 * @brief Alias for a filesystem path.
 */
using fspath = std::filesystem::path;

/**
 * @brief Structure representing an index, containing both forward and reverse
 * indices, configuration arguments, and the number of documents.
 */
struct Index {
    ForwardIndex docs_; /**< Map of document IDs to their corresponding text. */
    ReverseIndex
        entries_; /**< Map of terms to their occurrences in documents. */
    parser::ConfArgs conf_; /**< Configuration arguments for the parser. */
    size_t num_docs_ = 0;   /**< Number of documents in the index. */
};

/**
 * @brief Abstract base class for index writers.
 */
class IndexWriter {
   public:
    /**
     * @brief Writes the index to a specified path.
     * @param path Path where the index will be written.
     * @param index The index to be written.
     */
    virtual void write(const fspath& path, const Index& index) const = 0;

    /**
     * @brief Virtual destructor for IndexWriter.
     */
    virtual ~IndexWriter() = default;
};

/**
 * @brief Class for writing text-based indexes.
 */
class TextIndexWriter final : public IndexWriter {
   public:
    /**
     * @brief Writes the index to a specified path.
     * @param path Path where the index will be written.
     * @param index The index to be written.
     */
    void write(const fspath& path, const Index& index) const final;

    /**
     * @brief Generates a hash from a name.
     * @param name Name to be hashed.
     * @return Hashed string.
     */
    static std::string name_to_hash(const std::string& name);

   private:
    /**
     * @brief Writes the forward index to a specified path.
     * @param path Path where the index will be written.
     * @param forward_index The forward index to be written.
     * @return True if writing was successful, false otherwise.
     */
    static bool write_forward_index(const fspath& path,
                                    const ForwardIndex& forward_index);

    /**
     * @brief Writes the reverse index to a specified path.
     * @param path Path where the index will be written.
     * @param reverse_index The reverse index to be written.
     * @return True if writing was successful, false otherwise.
     */
    static bool write_reverse_index(const fspath& path,
                                    const ReverseIndex& reverse_index);

    /**
     * @brief Converts reverse index information to a string.
     * @param term Term for which the index information is retrieved.
     * @param idx_info Information about the term's occurrences.
     * @return String representation of the reverse index information.
     */
    static std::string reverse_index_info_to_str(
        const std::string& term,
        const std::map<size_t, std::vector<size_t>>& idx_info);
};

/**
 * @brief Class for building an index from parsed documents.
 */
class IndexBuilder final {
   public:
    /**
     * @brief Constructor for IndexBuilder.
     * @param args Configuration arguments for the parser.
     */
    explicit IndexBuilder(parser::ConfArgs args) {
        index_.conf_ = std::move(args);
    }

    /**
     * @brief Adds a document to the index.
     * @param document_id Unique identifier for the document.
     * @param text Text content of the document.
     */
    void add_document(size_t document_id, const std::string& text);

    /**
     * @brief Gets the built index.
     * @return The built index.
     */
    Index index() const;

   private:
    Index index_; /**< The index being built. */
};

}  // namespace fts::index
