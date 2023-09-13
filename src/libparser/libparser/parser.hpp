#pragma once

#include <filesystem>
#include <set>
#include <string>
#include <vector>

namespace parser {

struct ConfArgs {
    std::vector<std::string> stop_words;
    size_t ngram_min_length;
    size_t ngram_max_length;
};

using NgramVec = std::vector<std::vector<std::string>>;

ConfArgs parse_config(const std::string& path);  // filesystem::path
NgramVec parse_ngram(std::string str, const ConfArgs& args);

}  // namespace parser
