#include "commoninc.h"

using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "matrix.h"
#include "axesdata.h"
#include "drone.h"
#include "clientserver.h"
#include "commandargs.h"

#define _WHANT_FPE_

#ifdef _WHANT_FPE_
#include <fenv.h>
#endif

int g_Argc;
char **g_Argv;

_CommandArgs g_CmdArgs;

static void millisleep(int millisecs)
{
	timespec t;
	t.tv_sec = millisecs/1000;
	t.tv_nsec = (millisecs%1000)*1000*1000;
	nanosleep(&t,0);
}

int CommandCallback(
		void* context,
		int argc,
		char* argv[],
		SdCommand* command)
{
	Drone* drone = (Drone*)context;
	int err = EINVAL;
	if (!g_CmdArgs.ParseArgs(argc,argv)) {
		return err;
	}
	err = drone->ExecuteCommand(&g_CmdArgs);
	if (command) {
		*command = g_CmdArgs.GetCommand();
	}
	if (err < 0) {
		printf("Failed to execute command %d, err=%d\n",
			g_CmdArgs.GetCommand(), err);
	}
	return err;
}


int main(int argc, char *argv[])
{

#ifdef _WHANT_FPE_
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	g_Argc = argc;
	g_Argv = argv;

	int err = -1;
	ClientServer clientSrv;
	Drone* theDrone = Drone::Create();
	if (!theDrone) {
		err = ENOMEM;
		goto __return;
	}

	if (!g_CmdArgs.ParseArgs(argc,argv)) {
		printf("FAILED to parse the command line!\n\n");
		g_CmdArgs.PrintUsage();
		goto __return;
	}

	if (g_CmdArgs.GetArgc() == 1) {
		g_CmdArgs.PrintUsage();
		goto __return;
	}

	if (g_CmdArgs.IsRoleServer())
	{
		//
		// Daemonize, if not client as well.
		// Kick off a listening thread
		//
		err = clientSrv.StartServerThread(
				CommandCallback,
				(void*)theDrone,
				g_CmdArgs.GetServerPort());
		if (0 != err) {
			goto __return;
		}
	}

	if (g_CmdArgs.IsRoleClient() && !g_CmdArgs.IsRoleServer())
	{
		// Execute the client logic - serialize and send the command to the
		// server
		string cmdLine;
		clientSrv.SerializeCommand(&cmdLine,argc-1,argv+1);
		if (0 != (err = clientSrv.SendCommandToDrone(
				g_CmdArgs.GetHostAddress(),
				g_CmdArgs.GetServerPort(),
				cmdLine))) {
			goto __return;
		}
	}

	if (g_CmdArgs.IsRoleServer())
	{
		// execute the command that came with main
		theDrone->ExecuteCommand(&g_CmdArgs);

		// wait for server thread to terminate
		while (0 != clientSrv.IsServerThreadRunning()) {
			millisleep(1000);
			if (!theDrone->IsRunning()) {
				break;
			}
		}
		clientSrv.JoinServerThread();
	}

	__return:

	if (-1 == err) {
		err = errno;
	}

	if (theDrone) {
		delete theDrone;
	}

	return err;
}
