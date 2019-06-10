#!/bin/sh
loop=true
logFile="/var/log/serverstream-daemon.log"
pidFile="/var/run/serverstream-daemon.pid"
prog='/home/edwino/Workspace/tcc/serverstream/build/serverstream'
options="8888 8887 1024"
invocation="$prog $options"

while $loop; do

    echo "[$(date +'%H:%M:%S %d/%m/%Y')] Runnig '$invocation'" >> $logFile
    $invocation >> $logFile 2>&1 &

    pid=$!
    echo $pid > $pidFile
    echo "[$(date +'%H:%M:%S %d/%m/%Y')] PID: $pid" >> $logFile

    if [ $? -eq 0 ]; then
        wait $pid > /dev/null 2>&1
        if [ ! -f $pid ]; then
            running=false
            rm $pidFile > /dev/null 2>&1
            echo "[$(date +'%H:%M:%S %d/%m/%Y')] Service stopped" >> $logFile
        fi
    else
        echo "Unable to write PID file"
        echo "[$(date +'%H:%M:%S %d/%m/%Y')] Unable to write PID file" >> $logFile
        rm $pidFile > /dev/null 2>&1
        exit 1
    fi

    echo "" >> $logFile;
    echo "" >> $logFile;
done
