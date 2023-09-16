#include <libparser/parser.hpp>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <string>

TEST(parse_ngram_test, normal_case) {
    parser::ConfArgs args;
    args.stop_words = {"and", "dr", "mr"};
    args.ngram_min_length = 3;
    args.ngram_max_length = 6;

    std::string str = "Dr. Jekyll and Mr. Hyde";

    parser::NgramVec expected_result = {{"jek", "jeky", "jekyl", "jekyll"},
                                        {"hyd", "hyde"}};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_text) {
    parser::ConfArgs args;
    args.stop_words = {"and", "dr", "mr"};
    args.ngram_min_length = 3;
    args.ngram_max_length = 6;

    std::string str = "";

    parser::NgramVec expected_result = {};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_stop_words) {
    parser::ConfArgs args;
    args.stop_words = {};
    args.ngram_min_length = 2;
    args.ngram_max_length = 4;

    std::string str = "Testing ngram parser function";

    parser::NgramVec expected_result = {{"te", "tes", "test"},
                                        {"ng", "ngr", "ngra"},
                                        {"pa", "par", "pars"},
                                        {"fu", "fun", "func"}};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_ngrams) {
    parser::ConfArgs args;
    args.stop_words = {"testing", "function"};
    args.ngram_min_length = 10;
    args.ngram_max_length = 20;

    std::string str = "Testing ngram parser function";

    parser::NgramVec expected_result = {};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words) {
    parser::ConfArgs args;
    args.stop_words = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length = 1;
    args.ngram_max_length = 20;

    std::string str = "Testing ngram parser function";

    parser::NgramVec expected_result = {};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_puncts) {
    parser::ConfArgs args;
    args.stop_words = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length = 1;
    args.ngram_max_length = 20;

    std::string str = "?Testing, ngram! parser: function;";

    parser::NgramVec expected_result = {};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_many_spaces) {
    parser::ConfArgs args;
    args.stop_words = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length = 1;
    args.ngram_max_length = 20;

    std::string str =
        "              Testing            ngram   parser          function";

    parser::NgramVec expected_result = {};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, one_word_without_stop_words) {
    parser::ConfArgs args;
    args.stop_words = {};
    args.ngram_min_length = 1;
    args.ngram_max_length = 4;

    std::string str = "Testing ";

    parser::NgramVec expected_result = {{
        "t",
        "te",
        "tes",
        "test",
    }};

    auto result = parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}
