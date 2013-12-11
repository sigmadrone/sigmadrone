#include "commoninc.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/signal.h>

#include "filelock.h"
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
		SdCommandCode* command)
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

void fatal_error_signal (int sig)
{
	static int fatal_error_in_progress = 0;
	/* Since this handler is established for more than one kind of signal,
          it might still get invoked recursively by delivery of some other kind
          of signal.  Use a static variable to keep track of that. */
	if (fatal_error_in_progress) {
		raise (sig);
	}
	fatal_error_in_progress = 1;

	/*
	 * Now do the clean up actions:
	 * 		- reset terminal modes
	 * 		- kill child processes
	 * 		- remove lock files
     */

	printf("FATAL: error signal %d raised, shutting down the drone!\n",
			sig);
	Drone::Only()->Reset(0,true);

	/*
	 * Now reraise the signal.  We reactivate the signal's
	 * default handling, which is to terminate the process.
	 * We could just call exit or abort,
	 * but reraising the signal sets the return status
	 * from the process correctly.
	 */
	signal (sig, SIG_DFL);
	raise (sig);
}

int main(int argc, char *argv[])
{
	FileLock globalLock("sigmadrone");

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
		if (!globalLock.Lock()) {
			printf("Failed to obtain global lock: %s\n",strerror(errno));
			goto __return;
		}

		/*
		 * Register error signal handlers
		 */
		signal(SIGFPE,fatal_error_signal);
		signal(SIGILL,fatal_error_signal);
		signal(SIGSEGV,fatal_error_signal);
		signal(SIGBUS,fatal_error_signal);
		signal(SIGABRT,fatal_error_signal);

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
		if (0 == (err = theDrone->ExecuteCommand(&g_CmdArgs))) {
			// wait for server thread to terminate
			while (0 != clientSrv.IsServerThreadRunning()) {
				millisleep(1000);
				if (!theDrone->IsRunning()) {
					break;
				}
			}
			clientSrv.JoinServerThread();
		}
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
