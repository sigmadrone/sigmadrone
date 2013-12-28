#include <iostream>
#include "jsonrpcparser.h"
#include <string>

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
		currentTestName = 0;
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


void TestParseJsonRpcFromBuffer()
{
	TEST_BEGIN("TestParseJsonRpcFromBuffer");
	std::string jsonBuf;
	JsonRpcParser parser;
	uint32_t usedLen = 0;
	static const char* schema1 =
		"{"
		    "\"jsonrpc\": \"2.0\", \"method\": \"run\", \"params\": "
		    "{"
				"\"Flight\": "
				"{"
				 	 "\"Thrust\": 0.5,"
				 	 "\"MinThrust\":  0.3,"
				 	 "\"MaxThrust\": 0.7"
				"}"
			"}, "
			"\"id\": 0"
		"}";

	static const char* schema2 =
		"{"
		    "\"jsonrpc\": \"2.0\", \"method\": \"run\", \"params\": "
		    "{"
				"\"Flight\": "
				"{"
				 	 "\"Thrust\": 0.6,"
				 	 "\"MinThrust\":  0.4,"
				 	 "\"MaxThrust\": 0.8"
				"}"
			"}, "
			"\"id\": 0"
		"}";


	jsonBuf += schema1;

	if (!parser.ParseBuffer(jsonBuf.c_str(),jsonBuf.length(), &usedLen)) {
		TEST_FAILED();
		return;
	}

	if (usedLen != jsonBuf.length()) {
		TEST_FAILED();
		return;
	}

	double thrust=0, minThrust=0, maxThrust=0;
	parser.GetThrust(&thrust, &minThrust, &maxThrust);
	if (thrust != 0.5 || minThrust != 0.3 || maxThrust != 0.7) {
		TEST_FAILED();
		return;
	}

	/*
	 * Now add the second schema, parse, advance and then parse again
	 */
	jsonBuf += schema2;
	if (!parser.ParseBuffer(jsonBuf.c_str(),jsonBuf.length(), &usedLen)) {
		TEST_FAILED();
		return;
	}

	parser.GetThrust(&thrust, &minThrust, &maxThrust);
	if (thrust != 0.5 || minThrust != 0.3 || maxThrust != 0.7) {
		TEST_FAILED();
		return;
	}

	if (!parser.ParseBuffer(
			jsonBuf.c_str()+usedLen,
			jsonBuf.length()-usedLen,
			&usedLen)) {
		TEST_FAILED();
		return;
	}

	parser.GetThrust(&thrust, &minThrust, &maxThrust);
	if (thrust != 0.6 || minThrust != 0.4 || maxThrust != 0.8) {
		TEST_FAILED();
		return;
	}

	TEST_PASSED();
}

int main(int argc, char *argv[])
{
	TestParseJsonRpcFile();
	TestParseJsonRpcFromBuffer();

	TEST_STATS();

	return 0;
}
