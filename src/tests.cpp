#include <string>
#include <gtest/gtest.h>

namespace {
    TEST(SwiftTests, Test){
        EXPECT_EQ(1,1);
    }

}// namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
