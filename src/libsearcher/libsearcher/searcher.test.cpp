#include <libsearcher/searcher.hpp>

#include <gtest/gtest.h>

#include <filesystem>

#include <cmath>

static void rm_and_new_index(
    const std::unordered_map<size_t, std::string>& idx) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    std::filesystem::remove_all("index/");
    fts::index::IndexBuilder builder(conf);

    for (const auto& [doc_id, text] : idx) {
        builder.add_document(doc_id, text);
    }

    fts::index::TextIndexWriter writer;
    writer.write("./", builder.index());
}

double accuracy(double num, double eps = 5) {
    return round(num * pow(10, eps)) / pow(10, eps);
}

TEST(search_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index_accessor::TextIndexAccessor t_accs("./", conf);
    auto result = fts::searcher::search("bye earth", t_accs);
    fts::searcher::Results exp_result({{102, 3.29584}, {101, 1.09861}});

    ASSERT_EQ(exp_result.size(), result.size());

    for (size_t i = 0; i < exp_result.size(); ++i) {
        ASSERT_DOUBLE_EQ(accuracy(exp_result[i].second),
                         accuracy(result[i].second));
    }
}

TEST(search_test, one_word) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index_accessor::TextIndexAccessor t_accs("./", conf);
    auto result = fts::searcher::search("hello", t_accs);
    fts::searcher::Results exp_result({{100, 1.2164}, {102, 1.2164}});

    ASSERT_EQ(exp_result.size(), result.size());

    for (size_t i = 0; i < exp_result.size(); ++i) {
        ASSERT_DOUBLE_EQ(accuracy(exp_result[i].second),
                         accuracy(result[i].second));
    }
}

TEST(search_test, empty_query) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index_accessor::TextIndexAccessor t_accs("./", conf);
    auto result = fts::searcher::search("", t_accs);
    fts::searcher::Results exp_result{};

    ASSERT_EQ(exp_result.size(), result.size());

    for (size_t i = 0; i < exp_result.size(); ++i) {
        ASSERT_DOUBLE_EQ(accuracy(exp_result[i].second),
                         accuracy(result[i].second));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    rm_and_new_index(
        {{100, "Hello World"}, {101, "Bye World"}, {102, "Hello Earth"}});
    int ret = RUN_ALL_TESTS();
    std::filesystem::remove_all("index/");

    return ret;
}
