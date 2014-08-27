/*
 * dronerpcclient.h
 *
 *  Created on: Aug 17, 2014
 *      Author: svassilev
 */

#ifndef DRONERPCCLIENT_H_
#define DRONERPCCLIENT_H_

class SdJsonValue;
class DroneRpcClient {
public:
	DroneRpcClient();
	~DroneRpcClient();
	int ExecuteCommand(const CommandLineArgs& cmdArgs);

	static bool BuildJsonDroneConfigFromCmdLineArgs(
			SdJsonValue* jsonArgs,
			const CommandLineArgs& cmdArgs);
	static bool BuildJsonThrustParamsFromCmdLineArgs(
			SdJsonValue* jsonArgs,
			const CommandLineArgs&);
private:
};

#endif /* DRONERPCCLIENT_H_ */
