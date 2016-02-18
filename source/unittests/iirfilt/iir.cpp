// Step 1. Include necessary header files such that the stuff your
// test logic needs is declared.
//
// Don't forget gtest.h, which declares the testing framework.

#include <limits.h>
#include "gtest/gtest.h"
#include "iirfilt.h"
#include "fixedpoint.h"


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

	for (size_t i = 0; i < 10000; ++i) {
		iir.do_filter(-1);
	}

	EXPECT_LT(iir.output(),-0.99);
}

TEST(IirTest, fixedPoint)
{
	typedef QmnNumber<int32_t, 24> Q8dot24;
	static const double epsQ24 = Q8dot24::eps();

	// Test addition
	Q8dot24 qmn(0.5f);
	qmn += Q8dot24(1);
	double res = (float)qmn - 1.5f;
	EXPECT_LT(fabs(res),epsQ24);

	// basic multiplication
	qmn *= Q8dot24(3);
	EXPECT_EQ((float)qmn, 3 * 1.5f);

	// Addition with overflow - will saturate
	qmn = Q8dot24(127.1) + Q8dot24(0.999999);
	EXPECT_LT(128.0 - (double)qmn, 1.1 * epsQ24);

	// Addition with underflow - will saturate
	qmn = Q8dot24(-127.1) + Q8dot24(-0.999999);
	EXPECT_LT(128.0 + (double)qmn, 1.1 * epsQ24);

	// Subtraction with underflow - will saturate
	qmn = Q8dot24(-127.1) - Q8dot24(0.999999);
	EXPECT_LT(128.0 + (double)qmn, 1.1 * epsQ24);

	// Subtraction with overflow - will saturate
	qmn = Q8dot24(-127.1) + Q8dot24(-0.999999);
	EXPECT_LT(128.0 + (double)qmn, 1.1 * epsQ24);

	// Run of the mill subtraction
	res = Q8dot24(0.9999) - Q8dot24(0.0001);
	EXPECT_LT(res-0.9998, 1.1 * epsQ24);

	res = Q8dot24(0.9999) - Q8dot24(1.0);
	EXPECT_LT(res+0.0001, 1.1 * epsQ24);

	// Run of the mill multiplication
	res = Q8dot24(1.1) * Q8dot24(0.01);
	EXPECT_LT(fabs(res-0.011), 1.1 * epsQ24);

	res = Q8dot24(-1.1) * Q8dot24(0.02);
	EXPECT_LT(fabs(res+0.022), 1.1 * epsQ24);

	res = Q8dot24(1.111111) * Q8dot24(-0.03);
	EXPECT_LT(fabs(res + 1.111111*0.03), 2 * epsQ24);

	res = Q8dot24(-1.111111) * Q8dot24(-0.03);
	EXPECT_LT(fabs(res - 1.111111*0.03), 2 * epsQ24);

	// Multiplication with overflow - will saturate
	res = Q8dot24(127.9999999) * Q8dot24(1.1);
	EXPECT_LT(fabs(res-128), 1.1 * epsQ24);

	// Multiplication with underflow - will saturate
	res = Q8dot24(127.9999999) * Q8dot24(-1.1);
	EXPECT_LT(fabs(res+128), 1.1 * epsQ24);

	// Division
	res = Q8dot24(1.0) / Q8dot24(3.0);
	EXPECT_LT(fabs(res-1.0/3.0), 1.1 * epsQ24);

	res = Q8dot24(-1.00001) / Q8dot24(3.03030303);
	EXPECT_LT(fabs(res - (-1.00001/3.03030303)), 1.1 * epsQ24);

	Q1dot31 q31(1.0);
	static const double epsQ31 = Q1dot31::eps();
	res = 1.0 - (double)q31;
	EXPECT_LT(res, 1.1 * epsQ31);
	EXPECT_LT((double)(Q1dot31(1.0)+Q1dot31(-1)), epsQ31);
}

TEST(IirTest, fixedPointQ40)
{
	// Test addition
	Q24dot40 qmn(0.5f);
	qmn += 1;
	double res = qmn - 1.5f;
	EXPECT_LT(fabs(res),Q24dot40::eps() * 1.1);

	// basic multiplication
	qmn *= Q24dot40(3);
	EXPECT_EQ((double)qmn, 3 * 1.5f);

	// Addition
	qmn = Q24dot40(127.1) + Q24dot40(0.999999);
	EXPECT_LT(128.0 - (double)qmn, Q24dot40::eps() * 1.5);

	// Addition
	qmn = Q24dot40(-127.1) + Q24dot40(-0.999999);
	EXPECT_LT(128.0 + (double)qmn, Q24dot40::eps() * 1.5);

	// Subtraction
	qmn = Q24dot40(-127.1) - Q24dot40(0.999999);
	EXPECT_LT(128.0 + (double)qmn, Q24dot40::eps() * 1.5);

	qmn = Q24dot40(-127.1) + Q24dot40(-0.999999);
	EXPECT_LT(128.0 + (double)qmn, Q24dot40::eps() * 1.5);

	// Run of the mill subtraction
	res = Q24dot40(0.9999) - Q24dot40(0.0001);
	EXPECT_LT(res-0.9998, Q24dot40::eps() * 1.1);

	res = Q24dot40(0.9999) - Q24dot40(1.0);
	EXPECT_LT(res+0.0001, Q24dot40::eps() * 1.1);

	// Run of the mill multiplication
	res = Q24dot40(1.1) * Q24dot40(0.01);
	EXPECT_LT(fabs(res-0.011), Q24dot40::eps() * 2);

	res = Q24dot40(-1.1) * Q24dot40(0.02);
	EXPECT_LT(fabs(res+0.022), Q24dot40::eps() * 1.1);

	res = Q24dot40(1.111111) * Q24dot40(-0.03);
	EXPECT_LT(fabs(res + 1.111111*0.03), Q24dot40::eps() * 2);

	res = Q24dot40(-1.111111) * Q24dot40(-0.03);
	EXPECT_LT(fabs(res - 1.111111*0.03), Q24dot40::eps() * 2);

	res = Q24dot40(127.9999999) * Q24dot40(1.1);
	EXPECT_LT(fabs(res-127.9999999*1.1), Q24dot40::eps() * 100);

	res = Q24dot40(127.9999999) * Q24dot40(-1.1);
	EXPECT_LT(fabs(res+127.9999999*1.1), Q24dot40::eps() * 100);

#if 0
	// Division
	res = Q24dot40(1.0) / Q24dot40(3.0);
	EXPECT_LT(fabs(res-1.0/3.0), Q24dot40::eps() * 1.1);

	res = Q24dot40(-1.00001) / Q24dot40(3.03030303);
	EXPECT_LT(fabs(res - (-1.00001/3.03030303)), Q24dot40::eps() * 1.1);
#endif
}

typedef QmnNumber<int64_t, 48> Qmn;

TEST(IirTest, iirFixedPointMax1d) {
	IirLpf10HzFixedPoint<Qmn> iir;
	IirLpf10Hz1d iir1;
	for (float f = 0; f <= 1; f += 0.1) {
		iir.do_filter(Qmn(f));
		iir1.do_filter(f);
	}

	EXPECT_LT(fabs((double)iir.output()-iir1.output()), 0.003);//Qmn::eps() * 1.1);

	for (size_t i = 0; i < 10000; ++i) {
		iir.do_filter(Qmn(1.0));
	}
	double res = (double)(iir.output());
	EXPECT_LT(res,1.001);
	EXPECT_GT(res,0.999);

	for (size_t i = 0; i < 10000; ++i) {
		iir.do_filter(Qmn(-1.0));
	}

	EXPECT_LT((float)(iir.output()),-0.99);
}


TEST(IirTest, fixedPointmax3d) {
	typedef MatrixMN<Qmn, 3, 1> Vector3Qmn;
	IirLpf10HzFixedPoint<Vector3Qmn> iir;

	for (float f = 0; f <= 1; f += 0.1) {
		iir.do_filter(Vector3Qmn(f,f,f));
	}

	EXPECT_LT((double)iir.output()[0], 1.0);
	EXPECT_LT((double)iir.output()[1], 1.0);
	EXPECT_LT((double)iir.output()[2], 1.0);
	EXPECT_GT((double)iir.output()[0], 0.0);
	EXPECT_GT((double)iir.output()[1], 0.0);
	EXPECT_GT((double)iir.output()[2], 0.0);

	for (size_t i = 0; i < 1000; ++i) {
		iir.do_filter(Vector3Qmn(1.0,1.0,1.0));
	}

	EXPECT_LT((double)iir.output()[0],1.1);
	EXPECT_LT((double)iir.output()[1],1.1);
	EXPECT_LT((double)iir.output()[2],1.1);
	EXPECT_GT((double)iir.output()[0],0.99);
	EXPECT_GT((double)iir.output()[1],0.99);
	EXPECT_GT((double)iir.output()[2],0.99);
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
