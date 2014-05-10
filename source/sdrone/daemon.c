/*
 * Copyright (c) 2009-2012 The SigmaDrone developers
 *
 * Distributed under the MIT/X11 software license, see the accompanying
 * file COPYING or http://www.opensource.org/licenses/mit-license.php.
 *
 * Author: svassilev
 */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "daemon.h"

int daemon_init(void)
{
	pid_t pid;
	int fd, maxfd;

	pid = fork();
	if (pid < 0)
		return -1;
	else if (pid != 0)
		exit(0);	/* Parent exits */
	setsid();		/* Becomes session leader */
//	chdir("/");		/* Change working directory */
	umask(0);		/* Clear the file mode creation mask */
	/*
	 * Close all inherited file descriptors.
	 */
	if ((maxfd = sysconf(_SC_OPEN_MAX)) < 0)
		maxfd = 256;
	maxfd = 3;
	for (fd = 0; fd < maxfd; fd++) {
		close(fd);
	}
	return 0;
}
