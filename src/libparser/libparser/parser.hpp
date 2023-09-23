#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace fts::parser {

using StopWordsSet = std::unordered_set<std::string>;
using fspath = std::filesystem::path;

struct ConfArgs {
    StopWordsSet stop_words_ = {};
    size_t ngram_min_length_ = 0;
    size_t ngram_max_length_ = 0;
};

using NgramVec = std::vector<std::vector<std::string>>;

ConfArgs parse_config(const fspath& path);  // filesystem::path
NgramVec parse_ngram(std::string str, const ConfArgs& args);

}  // namespace fts::parser
