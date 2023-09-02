
#include <gtest/gtest.h>

#include <sum/sum.hpp>

#include <utility>
#include <vector>

struct Tests {
    int countA;
    int countB;
    int want;
};

using Tests = struct Tests;

TEST(sum_test, normal_case) {
    std::vector<Tests> tests = {
        {1, 2, 3},     {11, 22, 33},  {1000, 2, 1002}, {-1, 2, 1},
        {-11, 22, 11}, {-21, 3, -18}, {10, -2, 8},
    };

    for (auto& test : tests) {
        EXPECT_EQ(test.want, fts::sum(test.countA, test.countB));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
