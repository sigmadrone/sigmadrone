#!/bin/sh
DAEMON=/usr/bin/sigmad
NAME=sigmad
DESC="SIGMADRONE Daemon"
ARGS="-d"

test -f $DAEMON || exit 0

set -e

# source function library
. /etc/init.d/functions

delay_stop() {
	count=0
	while [ $count -lt 9 ] ; do
	        if pidof $DAEMON >/dev/null; then
	                sleep 1
	        else
	                return 0
	        fi
		count=`expr $count + 1`
	done
	echo "Failed to stop $DESC."
	return 1
}

case "$1" in
    start)
	echo -n "Starting $DESC: "
	start-stop-daemon -S -x $DAEMON -- $ARGS
	echo "$NAME."
	;;
    stop)
	echo -n "Stopping $DESC: "
	$DAEMON sd_exit
	echo "$NAME."
	;;
    restart)
	$0 stop
	delay_stop && $0 start
	;;
    *)
	echo "Usage: $0 {start|stop|restart}"
	;;
esac

exit 0
