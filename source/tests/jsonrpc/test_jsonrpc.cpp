#include <iostream>
#include "jsonrpcparser.h"

struct TestControl
{
	TestControl() {
		testsRun = testsPassed = 0;
		currentTestName = 0;
	}
	void OnTestBegin(const char* testName) {
		assert(0 == currentTestName);
		printf("--> TEST BEGIN: %s\n",testName);
		currentTestName = testName;
		++testsRun;
	}
	void OnTestFailed(
			int line,
			int err) {
		assert(currentTestName);
		printf("<-- TEST FAILED: %s, line %d, err %d\n",
				currentTestName,line,err);
		currentTestName= 0;
	}
	void OnTestPassed() {
		printf("<-- TEST PASSED: %s\n",currentTestName);
		++testsPassed;
	}
	void PrintStats() {
		printf("--> Tests RUN:    %d\n", testsRun);
		printf("--> Tests PASSED: %d\n", testsPassed);
		printf("--> Tests FAILED: %d\n", testsRun-testsPassed);
	}
private:
	const char* currentTestName;
	int testsRun;
	int testsPassed;
} g_TestControl;

#define TEST_BEGIN(name) g_TestControl.OnTestBegin(name)
#define TEST_FAILED() g_TestControl.OnTestFailed(__LINE__, EINVAL)
#define TEST_FAILED_ERR(err) g_TestControl.OnTestFailed(__LINE__,err)
#define TEST_PASSED() g_TestControl.OnTestPassed()
#define TEST_STATS() g_TestControl.PrintStats()

void TestParseJsonRpcFile()
{
	TEST_BEGIN("TestParseJsonRpcFile");
	JsonRpcParser parser;
	if (!parser.ParseFile("/home/svassilev/workspace/sigmadrone"
			"/doc/configuration/droneconfig.json")) {
		TEST_FAILED();
		return;
	}
	if (!parser.IsValidRpcSchema()) {
		TEST_FAILED();
		return;
	}

	SdDroneConfig config;
	memset(&config,0,sizeof(config));
	if (!parser.GetDroneConfig(&config)) {
		TEST_FAILED();
		return;
	}

	/*
	 * Spot check on parameters
	 */
	if (!config.Accel.DeviceName || 0 == config.Accel.SamplingRate ||
		!config.Gyro.DeviceName || 0 == config.Gyro.SamplingRate) {
		TEST_FAILED();
		return;
	}

	TEST_PASSED();
	return;
}

int main(int argc, char *argv[])
{
	TestParseJsonRpcFile();

	TEST_STATS();

	return 0;
}
