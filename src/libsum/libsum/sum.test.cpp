
#include <gtest/gtest.h>

#include <libsum/sum.hpp>

#include <vector>

struct Tests {
    int countA;
    int countB;
    int want;
};

TEST(sum_test, normal_case) {
    const std::vector<Tests> tests = {
        {1, 2, 3},     {11, 22, 33},  {1000, 2, 1002}, {-1, 2, 1},
        {-11, 22, 11}, {-21, 3, -18}, {10, -2, 8},
    };

    for (const auto& test : tests) {
        EXPECT_EQ(test.want, fts::sum(test.countA, test.countB));
    }
}
