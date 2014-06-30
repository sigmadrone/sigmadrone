#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "daemon.h"

int daemon_init(void)
{
	pid_t pid;
	int fd;

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
	return 0;
}
