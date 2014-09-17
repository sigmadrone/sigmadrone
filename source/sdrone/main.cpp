#include "commoninc.h"

#include "drone.h"
#include "commandargs.h"
#include "dronerpcclient.h"

#define _WHANT_FPE_

#ifdef _WHANT_FPE_
#include <fenv.h>
#endif

int g_Argc;
const char **g_Argv;

int main(int argc, const char *argv[])
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
	} else if (cmdArgs.IsRoleClient()) {
		/*
		 * Execute the client logic - send command to the server
		 */
		DroneRpcClient client;
		err = client.ExecuteCommand(cmdArgs);
	} else {
		printf("\nERROR: Server or client mode must be enabled!\n\n\n");
		goto __return;
	}

	err = 0;

	__return:

	if (-1 == err) {
		err = errno;
	}

	return err;
}
