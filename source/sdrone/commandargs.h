/*
 * _CommandArgs.h
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#ifndef COMMANDARGS_H_
#define COMMANDARGS_H_

#include "commoninc.h"
#include "../libcmdargs/cmdargs.h"

class CommandLineArgs
{
public:
	CommandLineArgs(int argc, const char* argv[]);
	~CommandLineArgs();
	void PrintUsage(int argc, const char* argv[]) const;
	bool GetThrust(double*) const;
	bool GetMinThrust(double*) const;
	bool GetMaxThrust(double*) const;
	double GetMinThrust() const;
	double GetMaxThrust() const;
	double GetDesiredThrust() const;
	QuaternionD GetTargetAttitude() const;
	bool IsHelpNeeded() const;
	bool IsRoleServer() const;
	bool IsDaemon() const;
	int GetServerPort() const;
	std::string GetHostAddress() const;
	std::string GetLogFile() const;
	std::vector<std::string> GetRpcParamsList() const;
	std::string GetRpcParamsAsJsonStream() const;
	inline SdCommandCode GetCommand() const { return _GetCommand(0); }
	std::string GetConfigFile() const {
		return m_cmdArgs.get_value("configfile");
	}
	bool GetMaxDps(int32_t* maxDps) const {
		return GetValueAsInt("maxdps",maxDps);
	}
	bool GetImuSamplingRage(int32_t* maxDps) const {
		return GetValueAsInt("rate",maxDps);
	}
	std::string GetImuInputFile() const {
		return m_cmdArgs.get_value("infile");
	}
	bool GetLogRate(double* doubleVal) const {
		return GetValueAsDouble("lograte",doubleVal);
	}
	bool LogRotMatrix() const {
		return GetBoolValue("rot-matrix");
	}
	bool GetImuToRotorAxisAngle(int32_t* intVal) const {
		return GetValueAsInt("imuangle",intVal);
	}
	bool GetKp(double* doubleVal) const {
		return GetValueAsDouble("Kp", doubleVal);
	}
	bool GetKi(double* doubleVal) const {
		return GetValueAsDouble("Ki", doubleVal);
	}
	bool GetKd(double* doubleVal) const {
		return GetValueAsDouble("Kd", doubleVal);
	}
	std::string GetApiName() const {
		return SdCommandCodeToString(FromCommanddLineArgToSdCommand(
				m_cmdArgs.get_value("apiname")));
	}
	bool GetGyroWatermark(int* value) const {
		return GetValueAsInt("gyrofifo",value);
	}
	bool GetAccelWatermark(int* value) const {
		return GetValueAsInt("accelfifo",value);
	}
private:

	static SdCommandCode FromCommanddLineArgToSdCommand(const std::string&);

	bool /*value present*/ GetValueAsDouble(const std::string& name, double* value) const;
	bool /*value present*/ GetValueAsInt(const std::string &name, int32_t* value) const;
	bool /*value*/ GetBoolValue(const std::string &name) const;

	SdCommandCode _GetCommand(std::vector<std::string>* rpcParams=0) const;
	inline bool IsValuePresent(const std::string& name) const {
		return m_cmdArgs.get_value(name).length() > 0;
	}

private:
	cmd_args m_cmdArgs;
	int m_argc;
	const char** m_argv;
};

#endif /* COMMANDARGS_H_ */
