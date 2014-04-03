/*
 * _CommandArgs.h
 *
 *  Created on: Sep 8, 2013
 *      Author: svassilev
 */

#ifndef COMMANDARGS_H_
#define COMMANDARGS_H_

#include "commoninc.h"
#include "cmdargs.h"
#include "jsonreadwrite.h"

class _CommandArgs: public CommandArgs
{
public:
	_CommandArgs();
	virtual ~_CommandArgs();
	void PrintUsage(int argc, char* argv[]) const;
	bool ParseArgs(int argc, char* argv[]);
	bool ParseJsonRpcArgs(const IJsonObject* args);
	SdCommandCode GetCommand() const;
	double GetMinThrust() const;
	double GetMaxThrust() const;
	double GetDesiredThrust() const;
	const QuaternionD* GetTargetAttitude() const;

	bool IsRoleServer() const;
	bool IsRoleClient() const;
	int GetServerPort() const { return m_ServerPort; }
	const char* GetHostAddress() const { return m_HostAddress; }
	const SdDroneConfig* GetDroneConfig() const { return &m_DroneCfg; }
	const IJsonObject* GetDroneConfigAsJobj();
	const IJsonObject* GetCommandArgsAsJobj();

	static bool ParseJsonDroneConfig(
			const IJsonObject* jsonRpcParams,
			SdDroneConfig*);
	static bool ParseJsonImuConfig(
			const IJsonObject* jsonImuObj,
			SdImuDeviceConfig*);
	static bool ParseJsonThrust(
			const IJsonObject* jsonRpcParams,
			double* thrust,
			double* minThrust,
			double* maxThrust);
	static bool BuildJsonDroneConfig(
			SdJsonObject* jsonDroneConfig,
			const SdDroneConfig&);
	static bool BuildJsonImuConfig(
			SdJsonObject* jsonImuConfig,
			const SdImuDeviceConfig&);
	static bool BuildJsonFlightParams(
			SdJsonObject* jsonFlightParams,
			double thrust,
			double minThrust,
			double maxThrust);
private:
	SdCommandCode m_Command;
	SdDroneConfig m_DroneCfg;
	double m_Thrust;
	double m_MinThrust;
	double m_MaxThrust;
	const char* m_HostAddress;
	const char* m_LogFile;
	int m_ServerPort;
	bool m_IsServer;
	bool m_IsClient;
	QuaternionD m_targetAttitude;
	CmdArgs m_cmdArgs;
	SdJsonObject m_jsonArgs;
};

#endif /* COMMANDARGS_H_ */
