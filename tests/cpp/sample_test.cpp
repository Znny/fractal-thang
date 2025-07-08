#include <gtest/gtest.h>

// Basic test to verify Google Test is working
TEST(SampleTest, BasicAssertions) {
    EXPECT_EQ(1 + 1, 2);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

// Test that we can include our project headers
#include "gen.h"

TEST(ProjectTest, CanIncludeHeaders) {
    // Test that our existing functions work (garbage code - will be removed)
    EXPECT_EQ(getMessage(42), "Result from C++: 42");
    
    // Test our struct functionality
    MyStruct testStruct = GenStruct(10, 20);
    EXPECT_EQ(testStruct.x, 10);
    EXPECT_EQ(testStruct.y, 20);
} 

 