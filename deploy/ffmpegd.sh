#!/bin/sh

### BEGIN INIT INFO
#
# Provides : ffmpeg-daemon
# Required-Start : $network
# Required-Stop  : $network
# Default-Start  : 2 3 4 5
# Default-Stop   : 0 1 6
# Short-Description : ffmpeg daemon
# Description : ffmpeg daemon
#
### END INIT INFO

invocation="serverstream/deploy/run-ffmpeg.sh"
pidFile="/var/run/ffmpeg-daemon.pid"

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run as root"
    exit 1
fi

start() {

    if [ -f $pidFile ]; then
        #get the pid and check if process exists
        pid=`cat $pidFile`
        kill -0 $pid > /dev/null 2>&1

        if [ $? -eq 0 ]; then
            echo "FFMPEG daemon is currently active"
            exit 1
        else
            rm $pidFile
        fi
    fi

    $invocation > /dev/null 2>&1 &
    sleep 0.5;
    if [ -f $pidFile ]; then
        echo "FFMPEG daemon started"
    else
        echo "Erro on FFMPEG daemon starts"
        exit 1
    fi
}

stop() {

    if [ -f $pidFile ]; then

        pid=`cat $pidFile`

        #send a INT signal
        kill -2 $pid > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo "FFMPEG daemon is not running"
            rm -r $pidFile > /dev/null 2>&1
            echo "PID file exists and was deleted"
        else
            echo "FFMPEG daemon stopped"
        fi
    else
        echo "FFMPEG daemon is not started"
        exit 1
    fi
}

case "$1" in
    start)
        start
        exit 0
    ;;
    stop)
        stop
        exit 0
    ;;
    restart)
        stop
        start
        exit 0
    ;;
    **)
        echo "Usage: $0 {start|stop|restart}" 1>&2
        exit 1
    ;;
esac
