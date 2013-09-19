/*
 * _CommandArgs.h
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#ifndef COMMANDARGS_H_
#define COMMANDARGS_H_

#include "commoninc.h"

struct ParsedArgs
{
	SdCommand Command;
	SdDroneConfig DroneCfg;
	double Thrust;
	double MinThrust;
	double MaxThrust;
	const char* HostAddress;
	const char* LogFile;
	int ServerPort;
	int TimeToSleep;
	bool IsServer;
	bool IsClient;
};

class _CommandArgs: public CommandArgs
{
public:
	_CommandArgs();
	virtual ~_CommandArgs();
	void PrintUsage();
	bool ParseArgs(int argc, char* argv[]);
	SdCommand GetCommand();
	const char* GetArgAsString(
			int index
			);
	int GetArgAsInt(
			int index
			);
	int64_t GetArgAsInt64(
			int index
			);
	double GetArgAsDouble(
			int index
			);
	int /*-1 or index*/ IsArgPresent(
			const char* argName);
	const char** GetArgv();
	int GetArgc();

	double GetMinThrust();
	double GetMaxThrust();
	double GetDesiredThrust();
	const QuaternionD* GetTargetAttitude();

	bool IsRoleServer();
	bool IsRoleClient();
	const ParsedArgs& GetParsedArgs() { return m_parsedArgs; }
	int GetServerPort() { return m_parsedArgs.ServerPort; }
	const char* GetHostAddress() { return m_parsedArgs.HostAddress; }
	int GetTimeToSleep() { return m_parsedArgs.TimeToSleep; }
	const SdDroneConfig* GetSdDroneConfig() { return &m_parsedArgs.DroneCfg; }
private:
	int m_argc;
	char** m_argv;
	ParsedArgs m_parsedArgs;
	QuaternionD m_targetAttitude;
};

#endif /* COMMANDARGS_H_ */
