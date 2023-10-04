#include <libsearcher/searcher.hpp>

#include <gtest/gtest.h>

#include <filesystem>

// fts::parser::ConfArgs conf = {
//     .stop_words_ = {"a",     "an",   "and",  "are", "as",    "at",   "be",
//                     "but",   "by",   "for",  "if",  "in",    "into", "is",
//                     "it",    "no",   "not",  "of",  "on",    "or",   "s",
//                     "such",  "t",    "that", "the", "their", "then", "there",
//                     "these", "they", "this", "to",  "was",   "will", "with"},
//     .ngram_min_length_ = 3,
//     .ngram_max_length_ = 6,
// };

// static void rm_and_new_index(
//     const std::unordered_map<size_t, std::string>& idx) {
//     std::filesystem::remove_all("index/");
//     fts::index::IndexBuilder builder(conf);

//     for (const auto& [doc_id, text] : idx) {
//         builder.add_document(doc_id, text);
//     }

//     fts::index::TextIndexWriter writer;
//     writer.write("./", builder.index());
// }

// TEST(search_test, normal_case) {
//     fts::index_accessor::TextIndexAccessor t_accs("./", conf);
//     auto result = fts::searcher::search("bye earth", t_accs);
//     fts::searcher::Results exp_result(
//         {{3.29584, 102}, {1.09861, 101}},
//         [](double key1, double key2) { return key1 > key2; });

//     auto it1 = result.begin();
//     auto it2 = exp_result.begin();
//     auto comparePairs = [](const auto& pair1, const auto& pair2) {
//         return std::abs(pair1.first - pair2.first) < 1e-5 &&
//                pair1.second == pair2.second;
//     };

//     while (it1 != result.end() && it2 != exp_result.end()) {
//         ASSERT_TRUE(comparePairs(*it1, *it2));
//         ++it1;
//         ++it2;
//     }

//     ASSERT_EQ(it1, result.end());
//     ASSERT_EQ(it2, exp_result.end());
// }

// TEST(search_test, one_word) {
//     fts::index_accessor::TextIndexAccessor t_accs("./", conf);
//     auto result = fts::searcher::search("hello", t_accs);
//     fts::searcher::Results exp_result(
//         {{1.2164, 100}, {1.2164, 102}},
//         [](double key1, double key2) { return key1 > key2; });

//     auto it1 = result.begin();
//     auto it2 = exp_result.begin();
//     auto comparePairs = [](const auto& pair1, const auto& pair2) {
//         return std::abs(pair1.first - pair2.first) < 1e-5 &&
//                pair1.second == pair2.second;
//     };

//     while (it1 != result.end() && it2 != exp_result.end()) {
//         ASSERT_TRUE(comparePairs(*it1, *it2));
//         ++it1;
//         ++it2;
//     }

//     ASSERT_EQ(it1, result.end());
//     ASSERT_EQ(it2, exp_result.end());
// }

// TEST(search_test, empty_query) {
//     fts::index_accessor::TextIndexAccessor t_accs("./", conf);
//     auto result = fts::searcher::search("", t_accs);
//     fts::searcher::Results exp_result(
//         [](double key1, double key2) { return key1 > key2; });

//     auto it1 = result.begin();
//     auto it2 = exp_result.begin();
//     auto comparePairs = [](const auto& pair1, const auto& pair2) {
//         return std::abs(pair1.first - pair2.first) < 1e-5 &&
//                pair1.second == pair2.second;
//     };

//     while (it1 != result.end() && it2 != exp_result.end()) {
//         ASSERT_TRUE(comparePairs(*it1, *it2));
//         ++it1;
//         ++it2;
//     }

//     ASSERT_EQ(it1, result.end());
//     ASSERT_EQ(it2, exp_result.end());
// }

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // rm_and_new_index(
    //     {{100, "Hello World"}, {101, "Bye World"}, {102, "Hello Earth"}});
    int ret = RUN_ALL_TESTS();
    std::filesystem::remove_all("index/");

    return ret;
}
