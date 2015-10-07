/*
 *  Sigmadrone
 *  Copyright (c) 2013-2015 The Sigmadrone Developers
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Martin Stoilov <martin@sigmadrone.org>
 *  Svetoslav Vassilev <svassilev@sigmadrone.org>
 */
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
