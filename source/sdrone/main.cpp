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
			SdJsonRpcReply rep;
			req.MethodName = SdCommandCodeToString(cmdArgs.GetCommand());

			switch (cmdArgs.GetCommand()) {
			case SD_COMMAND_RUN:
			case SD_COMMAND_RESET:
				req.Params = cmdArgs.GetCommandArgsAsJobj();
				break;
			default:break;
			}

			if (rpcDispatch.SendJsonRequest(req,
					cmdArgs.GetHostAddress(),
					cmdArgs.GetServerPort(),&rep))
			{
				printf("Successfully sent command %s, ret value %d\n",
						req.MethodName.c_str(), rep.ErrorCode);

				switch (cmdArgs.GetCommand()) {
				case SD_COMMAND_PING:
					if (rep.Results.GetType()==SD_JSONVALUE_STRING) {
						printf("Ping reply: %s\n", rep.Results.AsString().c_str());
					} else {
						printf("WARNING: Ping reply carries wrong type: %s\n",
								rep.Results.GetTypeAsString());
					}
					break;
				default:break;
				}
			}
			else
			{
				printf("Failed to send command %s to %s, error \"%s\"",
						req.MethodName.c_str(),cmdArgs.GetHostAddress(),
						strerror(err));
			}
			delete rpcTransport;
		}
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
