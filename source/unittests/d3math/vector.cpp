// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "gtest/gtest.h"
#include "d3math.h"

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.
TEST(VectorTest, min) {
	EXPECT_EQ(1, Vector3f(1,2,3).min());
	EXPECT_EQ(-9, Vector3f(1,2,-9).min());
	EXPECT_EQ(-9, Vector3f(-1,-2,-9).min());
}

TEST(VectorTest, max) {
	EXPECT_EQ(3, Vector3f(1,2,3).max());
	EXPECT_EQ(3, Vector3f(-9,2,3).max());
	EXPECT_EQ(-1, Vector3f(-1,-2,-3).max());
}

TEST(VectorTest, sum) {
	EXPECT_EQ(7, Vector3f(1,2,4).sum());
	EXPECT_EQ(-1, Vector3f(1,2,-4).sum());
	EXPECT_EQ(-7, Vector3f(-1,-2,-4).sum());
}

TEST(VectorTest, mulitplication) {
	EXPECT_EQ(7 * 3, (Vector3f(1,2,4) * 3).sum());
	EXPECT_EQ(-1 * 3, (Vector3f(1,2,-4) * 3).sum());
	EXPECT_EQ(-7 * 3, (Vector3f(-1,-2,-4) * 3).sum());
}


TEST(VectorTest, equal) {
	EXPECT_TRUE(Vector3f(1,2,3) == Vector3f(1,2,3));
	EXPECT_FALSE(Vector3f(1,2,3) != Vector3f(1,2,3));
	EXPECT_TRUE(Vector3f(1,2,3) != Vector3f(100,2,3));
}
