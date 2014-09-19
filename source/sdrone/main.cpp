#include "commoninc.h"

#include "drone.h"
#include "commandargs.h"
#include "dronerpcclient.h"

#define _WHANT_FPE_

#ifdef _WHANT_FPE_
#include <fenv.h>
#endif

int g_Argc;
char **g_Argv;

int main(int argc, char *argv[])
{
	CommandLineArgs cmdArgs(argc,argv);

#ifdef _WHANT_FPE_
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	g_Argc = argc;
	g_Argv = argv;

	int err = -1;
	if (cmdArgs.IsHelpNeeded() || argc==1) {
		cmdArgs.PrintUsage(argc,argv);
		goto __return;
	}

	if (cmdArgs.IsRoleServer()) {
		if (!Drone::Create()) {
			goto __return;
		}
		err = Drone::Only()->Run(cmdArgs);
		Drone::Destroy();
	} else {
		/*
		 * Execute the client logic - send command to the server
		 */
		err = DroneRpcClient().ExecuteCommand(cmdArgs);
	}

	__return:

	if (-1 == err) {
		err = errno;
	}

	return err;
}
