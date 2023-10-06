#include <libparser/parser.hpp>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <string>

TEST(parse_ngram_test, normal_case) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"and", "dr", "mr"};
    args.ngram_min_length_ = 3;
    args.ngram_max_length_ = 6;

    std::string str = "Dr. Jekyll and Mr. Hyde";

    fts::parser::NgramVec expected_result = {{"jek", "jeky", "jekyl", "jekyll"},
                                             {"hyd", "hyde"}};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_text) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"and", "dr", "mr"};
    args.ngram_min_length_ = 3;
    args.ngram_max_length_ = 6;

    std::string str;

    fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {};
    args.ngram_min_length_ = 2;
    args.ngram_max_length_ = 4;

    std::string str = "Testing ngram parser function";

    fts::parser::NgramVec expected_result = {{"te", "tes", "test"},
                                             {"ng", "ngr", "ngra"},
                                             {"pa", "par", "pars"},
                                             {"fu", "fun", "func"}};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_ngrams) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function"};
    args.ngram_min_length_ = 10;
    args.ngram_max_length_ = 20;

    std::string str = "Testing ngram parser function";

    fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    std::string str = "Testing ngram parser function";

    fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_puncts) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    std::string str = "?Testing, ngram! parser: function;";

    fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_many_spaces) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    std::string str =
        "              Testing            ngram   parser          function";

    fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, one_word_without_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 4;

    std::string str = "Testing ";

    fts::parser::NgramVec expected_result = {{
        "t",
        "te",
        "tes",
        "test",
    }};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}
