#include <libparser/parser.hpp>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace parser {

using VecWords = std::vector<std::string>;

ConfArgs parse_config(const std::string& path) {
    ConfArgs conf;
    std::ifstream fconf(path);

    if (!fconf.is_open()) {
        return conf;
    }

    nlohmann::json data;
    fconf >> data;
    fconf.close();

    conf.ngram_min_length = data["fts"]["parser"]["ngram_min_length"];
    conf.ngram_max_length = data["fts"]["parser"]["ngram_max_length"];
    std::copy(data["fts"]["parser"]["stop_words"].begin(),
              data["fts"]["parser"]["stop_words"].end(),
              std::back_inserter(conf.stop_words));

    return conf;
}

namespace {

void remove_puncts(std::string& str) {
    str.erase(std::remove_if(str.begin(), str.end(),
                             [](char chr) -> int { return std::ispunct(chr); }),
              str.end());
}

void tolower_str(std::string& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](const char chr) {
        return static_cast<char>(tolower(chr));
    });
}

void remove_short_words(VecWords& words, size_t min_len) {
    words.erase(std::remove_if(words.begin(), words.end(),
                               [&min_len](const std::string& str) -> bool {
                                   return str.size() < min_len;
                               }),
                words.end());
}

VecWords remove_stop_words(std::string& str, const VecWords& stop_words,
                           size_t min_len_word) {  //? unordered set
    VecWords words;
    std::string word;
    std::stringstream get_word(str);

    while (get_word >> word) {
        if (std::find(stop_words.begin(), stop_words.end(), word) ==
            stop_words.end()) {
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
        remove_stop_words(str, args.stop_words, args.ngram_min_length);

    NgramVec ngrams(words.size());

    int cnt_ngrams = 0;
    for (const auto& word : words) {
        if (word.size() >= args.ngram_min_length) {
            auto min_wlen_nlen = std::min(word.size(), args.ngram_max_length);
            for (size_t i = args.ngram_min_length; i <= min_wlen_nlen; ++i) {
                ngrams[cnt_ngrams].push_back(word.substr(0, i));
            }
            ++cnt_ngrams;
        }
    }
    ngrams.shrink_to_fit();
    return ngrams;
}

}  // namespace parser
