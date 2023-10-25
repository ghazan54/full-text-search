#include <libparser/parser.hpp>

#include <nlohmann/json.hpp>
#include <rapidcsv.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace fts::parser {

using VecWords = std::vector<std::string>;

ConfArgs parse_config(const fspath& path) {
    ConfArgs conf;

    try {
        std::ifstream fconf(path);
        if (!fconf.is_open()) {
            throw std::ios_base::failure("Could not open the config");
        }

        nlohmann::json data;
        fconf >> data;
        fconf.close();

        conf.ngram_min_length_ = data["fts"]["parser"]["ngram_min_length"];
        conf.ngram_max_length_ = data["fts"]["parser"]["ngram_max_length"];
        std::copy(data["fts"]["parser"]["stop_words"].begin(),
                  data["fts"]["parser"]["stop_words"].end(),
                  std::inserter(conf.stop_words_, conf.stop_words_.end()));
    } catch (const std::exception& e) {
        conf.ngram_min_length_ = 3;
        conf.ngram_max_length_ = 6;
        conf.stop_words_ = {};
        std::cerr << "parse_config: error when reading the config, default "
                     "values are used\n";
    }

    if (conf.ngram_min_length_ > conf.ngram_max_length_) {
        throw std::runtime_error(
            "parse_config: the minimum term length cannot be greater than the "
            "maximum");
    }

    return conf;
}

BooksInfo parse_csv(const fspath& path) {
    const rapidcsv::Document csv_file(path);
    BooksInfo books_info;
    std::vector<size_t> book_IDs = csv_file.GetColumn<size_t>("bookID");
    std::vector<std::string> titles = csv_file.GetColumn<std::string>("title");

    if (book_IDs.size() != titles.size()) {
        throw std::runtime_error("Error reading csv file.");
    }

    const size_t rows = book_IDs.size();
    for (size_t i = 0; i < rows; ++i) {
        BookInfo book_info;
        book_info.book_id = book_IDs[i];
        book_info.title = std::move(titles[i]);
        books_info.push_back(book_info);
    }

    return books_info;
}

namespace {

/**
 * @brief Removes punctuation characters from the input string.
 * @param str Input string to be modified.
 */
void remove_puncts(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(),
                             [](char chr) { return std::ispunct(chr); }),
              str.end());
}

/**
 * @brief Converts all characters in the input string to lowercase.
 * @param str Input string to be modified.
 */
void tolower_str(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](const char chr) {
        return static_cast<char>(tolower(chr));
    });
}

/**
 * @brief Removes words from the input vector that are shorter than a specified
 * length.
 * @param words Vector of words.
 * @param min_len Minimum length for a word to be kept.
 */
void remove_short_words(VecWords& words, size_t min_len) {
    words.erase(std::remove_if(words.begin(), words.end(),
                               [&min_len](const std::string& str) {
                                   return str.size() < min_len;
                               }),
                words.end());
}

/**
 * @brief Removes stop words from the input string and returns a vector of valid
 * words.
 * @param str Input string containing words.
 * @param stop_words Set of stop words.
 * @param min_len_word Minimum length for a valid word.
 * @return Vector of valid words.
 */
VecWords remove_stop_words(std::string& str, const StopWordsSet& stop_words,
                           size_t min_len_word) {
    VecWords words;
    std::string word;
    std::stringstream get_word(str);

    while (get_word >> word) {
        if (stop_words.find(word) == stop_words.end()) {
            words.push_back(word);
        }
    }

    remove_short_words(words, min_len_word);

    return words;
}

}  // namespace

NgramVec parse_ngram(std::string str, const ConfArgs& args) {
    remove_puncts(str);
    tolower_str(str);
    const auto words =
        remove_stop_words(str, args.stop_words_, args.ngram_min_length_);

    NgramVec ngrams(words.size());

    int cnt_ngrams = 0;
    for (const auto& word : words) {
        if (word.size() >= args.ngram_min_length_) {
            auto min_wlen_nlen = std::min(word.size(), args.ngram_max_length_);
            for (size_t i = args.ngram_min_length_; i <= min_wlen_nlen; ++i) {
                ngrams[cnt_ngrams].push_back(word.substr(0, i));
            }
            ++cnt_ngrams;
        }
    }

    return ngrams;
}

}  // namespace fts::parser
