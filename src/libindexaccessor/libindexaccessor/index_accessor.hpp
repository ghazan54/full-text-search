#pragma once

#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <filesystem>

#include <map>
#include <set>
#include <string>

namespace fts::index_accessor {

/**
 * @brief Alias for a filesystem path.
 */
using fspath = std::filesystem::path;

/**
 * @brief Abstract base class for index accessors.
 */
class IndexAccessor {
   public:
    /**
     * @brief Virtual destructor for IndexAccessor.
     */
    virtual ~IndexAccessor() = default;

    /**
     * @brief Retrieves information about a term from the reverse index.
     * @param term The term for which information is requested.
     * @return Reverse index information for the given term.
     */
    virtual index::ReverseIndex get_term_infos(
        const std::string& term) const = 0;

    /**
     * @brief Loads the text content of a document based on its ID.
     * @param document_id The ID of the document to load.
     * @return The text content of the specified document.
     */
    virtual std::string load_document(size_t document_id) const = 0;

    /**
     * @brief Retrieves the total number of documents in the index.
     * @return The total number of documents.
     */
    virtual size_t total_docs() const = 0;

    /**
     * @brief Retrieves the configuration arguments used for parsing.
     * @return Configuration arguments.
     */
    virtual parser::ConfArgs config() const = 0;
};

/**
 * @brief Class for accessing text-based indexes.
 */
class TextIndexAccessor final : public IndexAccessor {
   public:
    /**
     * @brief Constructor for TextIndexAccessor.
     * @param path Path to the directory containing the index.
     */
    explicit TextIndexAccessor(fspath path) : path_(std::move(path)) {
        const fspath conf_path(path_ / "index" / "config.json");
        if (!std::filesystem::exists(conf_path)) {
            throw std::ios_base::failure("Unable to find the config for index");
        }
        conf_ = parser::parse_config(conf_path);
    }

    /**
     * @brief Retrieves information about a term from the reverse index.
     * @param term The term for which information is requested.
     * @return Reverse index information for the given term.
     */
    index::ReverseIndex get_term_infos(const std::string& term) const override;

    /**
     * @brief Loads the text content of a document based on its ID.
     * @param document_id The ID of the document to load.
     * @return The text content of the specified document.
     */
    std::string load_document(size_t document_id) const override;

    /**
     * @brief Retrieves the total number of documents in the index.
     * @return The total number of documents.
     */
    size_t total_docs() const override;

    /**
     * @brief Retrieves the configuration arguments used for parsing.
     * @return Configuration arguments.
     */
    parser::ConfArgs config() const override;

   private:
    fspath path_;           /**< Path to the directory containing the index. */
    parser::ConfArgs conf_; /**< Configuration arguments for the parser. */
};

}  // namespace fts::index_accessor
