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

prog="/home/edwino/Workspace/tcc/serverstream/deploy/run-serverstream.sh"
pid_file="/var/run/serverstream-daemon.pid"

if [ "$(id -u)" != "0" ]; then
    echo "This script must be run as root"
    exit 1
fi

status() {
    if [ -f $pid_file ]; then
        ffmpeg_pid=`cat $pid_file`
        kill -0 $ffmpeg_pid > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo "serverstream daemon is currently active"
            exit 1
        else
            echo "serverstream daemon is not active"
        fi
    fi
}

start() {
    if [ -f $pid_file ]; then
        ffmpeg_pid=`cat $pid_file`
        kill -0 $ffmpeg_pid > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo "serverstream daemon is currently active"
            exit 1
        else
            rm $pid_file
        fi
    fi
    $prog > /dev/null 2>&1 &
    echo "serverstream daemon started"
}

stop() {
    if [ -f $pid_file ]; then
        ffmpeg_pid=`cat $pid_file`
        kill -2 $ffmpeg_pid > /dev/null 2>&1
        rm -r $pid_file > /dev/null 2>&1
        if [ $? -ne 0 ]; then
            echo "serverstream daemon is not running but PID file exists and was deleted"
        else
            echo "serverstream daemon stopped"
	fi
    else
        echo "serverstream daemon is not started"
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
    status)
        status()
        exit 0
    ;;
    **)
        echo "Usage: $0 {start|stop|restart|status}" 1>&2
        exit 1
    ;;
esac
