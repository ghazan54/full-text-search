#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace parser {

using StopWordsSet = std::unordered_set<std::string>;
using fspath = std::filesystem::path;

struct ConfArgs {
    StopWordsSet stop_words;
    size_t ngram_min_length;
    size_t ngram_max_length;
};

using NgramVec = std::vector<std::vector<std::string>>;

ConfArgs parse_config(const fspath& path);  // filesystem::path
NgramVec parse_ngram(std::string str, const ConfArgs& args);

}  // namespace parser
