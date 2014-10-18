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
