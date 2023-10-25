#pragma once

#include <libindex/index.hpp>
#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <filesystem>

#include <functional>
#include <string>
#include <vector>

namespace fts::searcher {

/**
 * @brief Structure representing a search result.
 */
struct Result {
    size_t doc_id; /**< ID of the matched document. */
    double score;  /**< Relevance score of the match. */
};

/**
 * @brief Alias for a vector of search results.
 */
using Results = std::vector<Result>;

/**
 * @brief Searches for matching documents based on a query.
 * @param query The search query.
 * @param index_accessor An IndexAccessor object providing access to the index.
 * @return A vector of search results.
 */
Results search(const std::string& query,
               index_accessor::IndexAccessor& index_accessor);

}  // namespace fts::searcher
