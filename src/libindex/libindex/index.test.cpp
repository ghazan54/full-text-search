#include <libindex/index.hpp>
#include <libparser/parser.hpp>

#include <gtest/gtest.h>

TEST(index_builder_test, normal_case) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index::Index index_exp;
    fts::index::IndexBuilder builder{conf};

    builder.add_document(199903, "The Matrix");
    builder.add_document(200305, "The Matrix Reloaded");
    builder.add_document(200311, "The Matrix Revolutions");

    index_exp.docs_ = {{199903, "The Matrix"},
                       {200305, "The Matrix Reloaded"},
                       {200311, "The Matrix Revolutions"}};

    index_exp.entries_ = {
        {"mat", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matr", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matri", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"matrix", {{199903, {0}}, {200305, {0}}, {200311, {0}}}},
        {"rel", {{200305, {1}}}},
        {"relo", {{200305, {1}}}},
        {"reloa", {{200305, {1}}}},
        {"reload", {{200305, {1}}}},
        {"rev", {{200311, {1}}}},
        {"revo", {{200311, {1}}}},
        {"revol", {{200311, {1}}}},
        {"revolu", {{200311, {1}}}}};

    ASSERT_EQ(builder.index().docs_, index_exp.docs_);
    ASSERT_EQ(builder.index().entries_, index_exp.entries_);

    index_exp.docs_.clear();
    index_exp.entries_.clear();
}

TEST(index_builder_test, empty_documents) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index::Index index_exp;
    fts::index::IndexBuilder builder{conf};

    index_exp.docs_ = {};

    index_exp.entries_ = {};

    ASSERT_EQ(builder.index().docs_, index_exp.docs_);
    ASSERT_EQ(builder.index().entries_, index_exp.entries_);

    index_exp.docs_.clear();
    index_exp.entries_.clear();
}

TEST(index_builder_test, only_stop_words) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index::Index index_exp;
    fts::index::IndexBuilder builder{conf};

    builder.add_document(1, "The and or");
    builder.add_document(2, "Into by");
    builder.add_document(3, "No such they");

    index_exp.docs_ = {{1, "The and or"}, {2, "Into by"}, {3, "No such they"}};

    index_exp.entries_ = {};

    ASSERT_EQ(builder.index().docs_, index_exp.docs_);
    ASSERT_EQ(builder.index().entries_, index_exp.entries_);

    index_exp.docs_.clear();
    index_exp.entries_.clear();
}

TEST(index_builder_test, repeat_word) {
    const fts::parser::ConfArgs conf = {
        {"a",     "an",   "and",  "are", "as",    "at",   "be",
         "but",   "by",   "for",  "if",  "in",    "into", "is",
         "it",    "no",   "not",  "of",  "on",    "or",   "s",
         "such",  "t",    "that", "the", "their", "then", "there",
         "these", "they", "this", "to",  "was",   "will", "with"},
        3,  // ngram_min_length_
        6   // ngram_max_length_
    };

    fts::index::Index index_exp;
    fts::index::IndexBuilder builder{conf};

    builder.add_document(1, "Matrix Matrix Matrix Matrix Matrix");

    index_exp.docs_ = {{1, "Matrix Matrix Matrix Matrix Matrix"}};

    index_exp.entries_ = {{"mat", {{1, {0, 1, 2, 3, 4}}}},
                          {"matr", {{1, {0, 1, 2, 3, 4}}}},
                          {"matri", {{1, {0, 1, 2, 3, 4}}}},
                          {"matrix", {{1, {0, 1, 2, 3, 4}}}}};

    ASSERT_EQ(builder.index().docs_, index_exp.docs_);
    ASSERT_EQ(builder.index().entries_, index_exp.entries_);

    index_exp.docs_.clear();
    index_exp.entries_.clear();
}
