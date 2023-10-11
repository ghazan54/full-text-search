#include <libparser/parser.hpp>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <string>

#include <fstream>

TEST(parse_ngram_test, normal_case) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"and", "dr", "mr"};
    args.ngram_min_length_ = 3;
    args.ngram_max_length_ = 6;

    const std::string str = "Dr. Jekyll and Mr. Hyde";

    const fts::parser::NgramVec expected_result = {
        {"jek", "jeky", "jekyl", "jekyll"}, {"hyd", "hyde"}};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_text) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"and", "dr", "mr"};
    args.ngram_min_length_ = 3;
    args.ngram_max_length_ = 6;

    const std::string str;

    const fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, empty_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {};
    args.ngram_min_length_ = 2;
    args.ngram_max_length_ = 4;

    const std::string str = "Testing ngram parser function";

    const fts::parser::NgramVec expected_result = {{"te", "tes", "test"},
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

    const std::string str = "Testing ngram parser function";

    const fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    const std::string str = "Testing ngram parser function";

    const fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_puncts) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    const std::string str = "?Testing, ngram! parser: function;";

    const fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, only_stop_words_and_many_spaces) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {"testing", "function", "ngram", "parser"};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 20;

    const std::string str =
        "              Testing            ngram   parser          function";

    const fts::parser::NgramVec expected_result = {};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

TEST(parse_ngram_test, one_word_without_stop_words) {
    fts::parser::ConfArgs args;
    args.stop_words_ = {};
    args.ngram_min_length_ = 1;
    args.ngram_max_length_ = 4;

    const std::string str = "Testing ";

    const fts::parser::NgramVec expected_result = {{
        "t",
        "te",
        "tes",
        "test",
    }};

    auto result = fts::parser::parse_ngram(str, args);
    EXPECT_EQ(result, expected_result);
}

// ----------------------- parse_config_test -----------------------

TEST(parse_config_test, valid_config) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath config_path = temp_dir / "valid_config.json";

    std::ofstream config_file(config_path);
    config_file
        << R"({"fts": {"parser": {"ngram_min_length": 2, "ngram_max_length": 5, "stop_words": ["word1", "word2", "word3"]}}})";
    config_file.close();

    const auto result = fts::parser::parse_config(config_path);
    const fts::parser::ConfArgs exp_result = {
        {"word1", "word2", "word3"}, 2, 5};

    ASSERT_EQ(result.ngram_min_length_, exp_result.ngram_min_length_);
    ASSERT_EQ(result.ngram_max_length_, exp_result.ngram_max_length_);
    ASSERT_EQ(result.stop_words_, exp_result.stop_words_);
}

TEST(parse_config_test, invalid_path) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath invalid_path = temp_dir / "nonexist_config.json";

    const auto result = fts::parser::parse_config(invalid_path);
    const fts::parser::ConfArgs exp_result = {{}, 3, 6};

    ASSERT_EQ(result.ngram_min_length_, 3);
    ASSERT_EQ(result.ngram_max_length_, 6);
    ASSERT_TRUE(result.stop_words_.empty());
}

TEST(parse_config_test, error_read_config) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath config_path = temp_dir / "invalid_config.json";

    std::ofstream config_file(config_path);
    config_file
        << R"({"fts": {"parser": {"ngram_min_length": 2, "ngram_max_length": ["word1", "word2", "word3"]}}})";
    config_file.close();

    const auto result = fts::parser::parse_config(config_path);
    const fts::parser::ConfArgs exp_result = {{}, 3, 6};

    ASSERT_EQ(result.ngram_min_length_, exp_result.ngram_min_length_);
    ASSERT_EQ(result.ngram_max_length_, exp_result.ngram_max_length_);
    ASSERT_EQ(result.stop_words_, exp_result.stop_words_);
}

TEST(parse_config_test, invalid_config_values) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath config_path = temp_dir / "invalid_config.json";

    std::ofstream config_file(config_path);
    config_file
        << R"({"fts": {"parser": {"ngram_min_length": 6, "ngram_max_length": 3, "stop_words": ["word1", "word2", "word3"]}}})";
    config_file.close();

    ASSERT_THROW(fts::parser::parse_config(config_path), std::runtime_error);
}

// ----------------------- parse_csv_test -----------------------

TEST(parse_csv_test, valid_csv) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath book_path = temp_dir / "valid_books.csv";

    std::ofstream csv_file(book_path);
    csv_file << "bookID,title\n";
    csv_file << "1,Book 1\n";
    csv_file << "2,Book 2\n";
    csv_file << "3,Book 3\n";
    csv_file.close();

    const auto result = fts::parser::parse_csv(book_path);
    const fts::parser::BooksInfo exp_result = {
        {1, "Book 1"}, {2, "Book 2"}, {3, "Book 3"}};

    ASSERT_EQ(result[0].book_id, exp_result[0].book_id);
    ASSERT_EQ(result[1].book_id, exp_result[1].book_id);
    ASSERT_EQ(result[2].book_id, exp_result[2].book_id);

    ASSERT_EQ(result[0].title, exp_result[0].title);
    ASSERT_EQ(result[1].title, exp_result[1].title);
    ASSERT_EQ(result[2].title, exp_result[2].title);
}

TEST(parse_csv_test, invalid_csv) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath book_path = temp_dir / "invalid_books.csv";

    std::ofstream csv_file(book_path);
    csv_file << "bookID,title\n";
    csv_file << "1,Book 1\n";
    csv_file << "2,Book 2\n";
    csv_file << "3\n";
    csv_file.close();

    ASSERT_THROW(fts::parser::parse_csv(book_path), std::out_of_range);
}

TEST(parse_csv_test, empty_csv) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath book_path = temp_dir / "empty_books.csv";

    std::ofstream csv_file(book_path);
    csv_file.close();

    ASSERT_THROW(fts::parser::parse_csv(book_path), std::out_of_range);
}

TEST(parse_csv_test, nonexist_csv) {
    const fts::parser::fspath temp_dir = std::filesystem::temp_directory_path();
    const fts::parser::fspath invalid_path = temp_dir / "nonexistent_file.csv";

    ASSERT_THROW(fts::parser::parse_csv(invalid_path), std::runtime_error);
}
