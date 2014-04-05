#include "commoninc.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/file.h>

#include "drone.h"
#include "commandargs.h"
#include "jsonrpcdispatch.h"

#define _WHANT_FPE_

#ifdef _WHANT_FPE_
#include <fenv.h>
#endif

int g_Argc;
char **g_Argv;

int main(int argc, char *argv[])
{
	_CommandArgs cmdArgs;

#ifdef _WHANT_FPE_
	feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
#endif

	g_Argc = argc;
	g_Argv = argv;

	int err = -1;
	if (!cmdArgs.ParseArgs(argc,argv)) {
		cmdArgs.PrintUsage(argc,argv);
		goto __return;
	}

	if (argc == 1) {
		cmdArgs.PrintUsage(argc,argv);
		goto __return;
	}

	if (cmdArgs.IsRoleServer()) {
		if (!Drone::Create()) {
			goto __return;
		}
		Drone::Only()->Run(cmdArgs);
		Drone::Destroy();
	} else if (cmdArgs.IsRoleClient()) {
		/*
		 * Execute the client logic - send command to the server
		 */
		IRpcTransport* rpcTransport=IRpcTransport::Create(
				IRpcTransport::TRANSPORT_HTTP);
		if (rpcTransport)
		{
			SdJsonRpcDispatcher rpcDispatch(rpcTransport);
			SdJsonRpcRequest req;
			req.MethodName = SdCommandCodeToString(cmdArgs.GetCommand());
			req.Params = cmdArgs.GetCommandArgsAsJobj();
			if (!rpcDispatch.SendJsonRequest(req,
					cmdArgs.GetHostAddress(),
					cmdArgs.GetServerPort(),0))
			{
				printf("Failed to send command %s to %s, error \"%s\"",
						req.MethodName.c_str(),cmdArgs.GetHostAddress(),
						strerror(err));
			}
			delete rpcTransport;
		}
	} else {
		printf("\nERROR: Server or client mode must be enabled!\n\n\n");
		cmdArgs.PrintUsage(argc,argv);
		goto __return;
	}

	err = 0;

	__return:

	if (-1 == err) {
		err = errno;
	}

	return err;
}
