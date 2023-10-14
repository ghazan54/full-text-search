#include <libindexaccessor/index_accessor.hpp>
#include <libparser/parser.hpp>

#include <gtest/gtest.h>

namespace {
void rm_and_new_index(const std::unordered_map<size_t, std::string>& idx) {
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

    const fts::index::TextIndexWriter writer;
    writer.write("./", builder.index());
}
}  // namespace

TEST(index_accessor_load_document_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor("./", conf);
    {
        const auto result = accessor.load_document(199903);
        const std::string exp_result("The Matrix");
        ASSERT_EQ(exp_result, result);
    }
    {
        const auto result = accessor.load_document(200305);
        const std::string exp_result("The Matrix Reloaded");
        ASSERT_EQ(exp_result, result);
    }

    {
        const auto result = accessor.load_document(200311);
        const std::string exp_result("The Matrix Revolutions");
        ASSERT_EQ(exp_result, result);
    }
}

TEST(index_accessor_load_document_test, empty_dir) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor(
        "non-existent directory", conf);

    bool result = false;

    try {
        accessor.load_document(199903);
    } catch (const std::exception& e) {
        result = true;
    }

    ASSERT_TRUE(result);
}

TEST(index_accessor_total_docs_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor("./", conf);
    const auto result = accessor.total_docs();
    const size_t exp_result = 3;

    ASSERT_EQ(exp_result, result);
}

TEST(index_accessor_total_docs_test, empty_dir) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor(
        "non-existent directory", conf);

    bool result = false;

    try {
        accessor.total_docs();

    } catch (const std::exception& e) {
        result = true;
    }

    ASSERT_TRUE(result);
}

TEST(index_accessor_config_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor("./", conf);
    const auto result = accessor.config();

    ASSERT_EQ(conf.ngram_min_length_, result.ngram_min_length_);
    ASSERT_EQ(conf.ngram_max_length_, result.ngram_max_length_);
    ASSERT_EQ(conf.stop_words_, result.stop_words_);
}

TEST(index_accessor_config_test, empty_dir) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    const fts::index_accessor::TextIndexAccessor accessor(
        "non-existent directory", conf);
    const auto result = accessor.config();

    ASSERT_EQ(conf.ngram_min_length_, result.ngram_min_length_);
    ASSERT_EQ(conf.ngram_max_length_, result.ngram_max_length_);
    ASSERT_EQ(conf.stop_words_, result.stop_words_);
}

TEST(index_accessor_get_term_infos_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index_accessor::TextIndexAccessor accessor("./", conf);
    const auto result = accessor.get_term_infos("matrix");
    const fts::index::ReverseIndex exp_result = {
        {"matrix", {{199903, {0}}, {200305, {0}}, {200311, {0}}}}};

    ASSERT_EQ(exp_result, result);
}

TEST(index_accessor_get_term_infos_test, empty_dir) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index_accessor::TextIndexAccessor accessor("non-existent directory",
                                                    conf);
    const auto result = accessor.get_term_infos("matrix");
    const fts::index::ReverseIndex exp_result = {};

    ASSERT_EQ(exp_result, result);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    rm_and_new_index({{199903, "The Matrix"},
                      {200305, "The Matrix Reloaded"},
                      {200311, "The Matrix Revolutions"}});
    const int ret = RUN_ALL_TESTS();
    std::filesystem::remove_all("index/");

    return ret;
}
