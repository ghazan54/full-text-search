#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace fts::parser {

/**
 * @brief Alias for an unordered set of strings representing stop words.
 */
using StopWordsSet = std::unordered_set<std::string>;

/**
 * @brief Alias for a filesystem path.
 */
using fspath = std::filesystem::path;

/**
 * @brief Structure holding configuration arguments.
 */
struct ConfArgs {
    StopWordsSet stop_words_ = {}; /**< Set of stop words. */
    size_t ngram_min_length_ = 0;  /**< Minimum length of n-grams. */
    size_t ngram_max_length_ = 0;  /**< Maximum length of n-grams. */
};

/**
 * @brief Alias for a vector of vectors of strings representing n-grams.
 */
using NgramVec = std::vector<std::vector<std::string>>;

/**
 * @brief Structure holding information about a book.
 */
struct BookInfo {
    size_t book_id = 0; /**< Unique identifier for the book. */
    std::string title;  /**< Title of the book. */
};

/**
 * @brief Alias for a vector of BookInfo structures holding information about
 * multiple books.
 */
using BooksInfo = std::vector<BookInfo>;

/**
 * @brief Parses the configuration file.
 * @param path Path to the configuration file.
 * @return Configuration arguments.
 */
ConfArgs parse_config(const fspath& path);

/**
 * @brief Parses a CSV file to retrieve information about books.
 * @param path Path to the CSV file.
 * @return Vector of BookInfo structures.
 */
BooksInfo parse_csv(const fspath& path);

/**
 * @brief Parses a string and generates n-grams based on configuration
 * arguments.
 * @param str Input string.
 * @param args Configuration arguments.
 * @return Vector of vectors of strings representing n-grams.
 */
NgramVec parse_ngram(std::string str, const ConfArgs& args);

}  // namespace fts::parser
