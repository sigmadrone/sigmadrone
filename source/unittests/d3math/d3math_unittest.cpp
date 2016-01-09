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
TEST(MatrixTest, min) {
	EXPECT_EQ(1, Matrix3f(1,2,3,4,5,6,7,8,9).min());
	EXPECT_EQ(-9, Matrix3f(1,2,3,4,5,6,7,8,-9).min());
	EXPECT_EQ(-9, Matrix3f(-1,-2,-3,-4,-5,-6,-7,-8,-9).min());
}

TEST(MatrixTest, max) {
	EXPECT_EQ(9, Matrix3f(1,2,3,4,5,6,7,8,9).max());
	EXPECT_EQ(8, Matrix3f(1,2,3,4,5,6,7,8,-9).max());
	EXPECT_EQ(-1, Matrix3f(-1,-2,-3,-4,-5,-6,-7,-8,-9).max());
}

TEST(MatrixTest, sum) {
	EXPECT_EQ(45, Matrix3f(1,2,3,4,5,6,7,8,9).sum());
	EXPECT_EQ(27, Matrix3f(1,2,3,4,5,6,7,8,-9).sum());
	EXPECT_EQ(-45, Matrix3f(-1,-2,-3,-4,-5,-6,-7,-8,-9).sum());
}

TEST(MatrixTest, mulitplication) {
	EXPECT_EQ(45 * 3, (Matrix3f(1,2,3,4,5,6,7,8,9) * 3).sum());
	EXPECT_EQ(27 * 3, (Matrix3f(1,2,3,4,5,6,7,8,-9) * 3).sum());
	EXPECT_EQ(-45 * 3, (Matrix3f(-1,-2,-3,-4,-5,-6,-7,-8,-9) * 3).sum());
}

TEST(MatrixTest, division) {
	EXPECT_EQ(45 / 3, (Matrix3f(1,2,3,4,5,6,7,8,9) / 3).sum());
	EXPECT_EQ(27 / 3, (Matrix3f(1,2,3,4,5,6,7,8,-9) / 3).sum());
	EXPECT_EQ(-45 / 3, (Matrix3f(-1,-2,-3,-4,-5,-6,-7,-8,-9) / 3).sum());
}

TEST(MatrixTest, equal) {
	EXPECT_TRUE(Matrix3f(1,2,3,4,5,6,7,8,9) == Matrix3f(1,2,3,4,5,6,7,8,9));
	EXPECT_FALSE(Matrix3f(1,2,3,4,5,6,7,8,9) != Matrix3f(1,2,3,4,5,6,7,8,9));
	EXPECT_TRUE(Matrix3f(1,2,3,4,5,6,7,8,9) != Matrix3f(100,2,3,4,5,6,7,8,9));
}
