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
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "daemon.h"

static void daemon_block_thread_signals()
{
	sigset_t set;

	sigemptyset(&set);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGHUP);
	sigaddset(&set, SIGQUIT);
	pthread_sigmask(SIG_BLOCK, &set, NULL);
}

int daemon_init(void)
{
	pid_t pid;
	int fd;

	pid = fork();
	if (pid < 0)
		return -1;
	else if (pid != 0)
		exit(0);	/* Parent exits */
	pid = fork();
	if (pid < 0)
		return -1;
	else if (pid != 0)
		exit(0);	/* Parent exits */
	setsid();		/* Becomes session leader */
//	chdir("/");		/* Change working directory */
	umask(0);		/* Clear the file mode creation mask */
	for (fd = 0; fd < 3; fd++) {
		close(fd);
	}

	daemon_block_thread_signals();
	return 0;
}
