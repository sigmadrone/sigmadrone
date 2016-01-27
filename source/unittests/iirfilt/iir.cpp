// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "gtest/gtest.h"
#include "iirfilt.h"


typedef IirLowPassFilter5Hz<Vector3d, double> IirLpf10Hz3d;
typedef IirLowPassFilter5Hz<double, double> IirLpf10Hz1d;

// Step 2. Use the TEST macro to define your tests.
//
// TEST has two parameters: the test case name and the test name.
// After using the macro, you should define your test logic between a
// pair of braces.  You can use a bunch of macros to indicate the
// success or failure of a test.  EXPECT_TRUE and EXPECT_EQ are
// examples of such macros.  For a complete list, see gtest.h.

TEST(IirTest, max1d) {
	IirLpf10Hz1d iir;
	for (float f = 0; f <= 1; f += 0.1) {
		iir.do_filter(f);
	}
	EXPECT_LT(iir.output(), 1.0);
	EXPECT_GT(iir.output(), 0.0);

	for (size_t i = 0; i < 1000; ++i) {
		iir.do_filter(1);
	}

	EXPECT_LT(iir.output(),1.1);
	EXPECT_GT(iir.output(),0.99);
}

TEST(IirTest, max3d) {
	IirLpf10Hz3d iir;
	for (float f = 0; f <= 1; f += 0.1) {
		iir.do_filter(Vector3d(f,f,f));
	}
	EXPECT_LT(iir.output()[0], 1.0);
	EXPECT_LT(iir.output()[1], 1.0);
	EXPECT_LT(iir.output()[2], 1.0);
	EXPECT_GT(iir.output()[0], 0.0);
	EXPECT_GT(iir.output()[1], 0.0);
	EXPECT_GT(iir.output()[2], 0.0);

	for (size_t i = 0; i < 1000; ++i) {
		iir.do_filter(Vector3d(1,1,1));
	}

	EXPECT_LT(iir.output()[0],1.1);
	EXPECT_LT(iir.output()[1],1.1);
	EXPECT_LT(iir.output()[2],1.1);
	EXPECT_GT(iir.output()[0],0.99);
	EXPECT_GT(iir.output()[1],0.99);
	EXPECT_GT(iir.output()[2],0.99);
}
