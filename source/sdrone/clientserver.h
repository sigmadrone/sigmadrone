/*
 * clientserver.h
 *
 *  Created on: Jun 17, 2013
 *      Author: svassilev
 */

#ifndef COMMANDCONTROL_H_
#define COMMANDCONTROL_H_

#include "commoninc.h"
#include <pthread.h>
#include <string>
using namespace std;

typedef int (*ExecuteCommandCallbackT)(
		void* context,
		int argc,
		char* argv[],
		SdCommand* cmd
		);

class ClientServer {
public:
	ClientServer();
	~ClientServer();
	int StartServerThread(
			ExecuteCommandCallbackT callback,
			void* context,
			int port);
	void StopServerThread();
	void JoinServerThread();
	int SendCommandToDrone(
			const char* host,
			int port,
			const string& command
			);
	bool IsServerThreadRunning();
	static void SerializeCommand(
			string* cmdLine,
			int argc,
			char** argv
			);
	static char** DeserializeCommand(
			char* buffer,
			int* _argc
			);
private:
	static void* ServerThread(void* arg);

private:
	pthread_t m_ServerThread;
	ExecuteCommandCallbackT m_CommandCallback;
	void* m_CallbackContext;
	int m_ServerPort;
	bool m_IsReceivingCommands;
};

#endif /* COMMANDCONTROL_H_ */
