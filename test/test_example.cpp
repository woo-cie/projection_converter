#include <gtest/gtest.h>

class ExampleTest : public ::testing::Test {
protected:
  virtual void SetUp() {}
  virtual void TearDown() {}
};

// Demonstrate some basic assertions.
TEST_F(ExampleTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}
