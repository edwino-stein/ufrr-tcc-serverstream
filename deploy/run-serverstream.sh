#!/bin/sh
logFile="/var/log/serverstream-daemon.log"
pidFile="/var/run/serverstream-daemon.pid"
prog='serverstream/build/serverstream'
options="8088 8087 1024 stats.json"
invocation="$prog $options"

pid=$$
echo $pid > $pidFile
if [ $? -ne 0 ]; then
    echo "Unable to write PID file"
    echo "[$(date +'%H:%M:%S %d/%m/%Y')] Unable to write PID file" >> $logFile
    rm $pidFile > /dev/null 2>&1
    exit 1
fi

echo "[$(date +'%H:%M:%S %d/%m/%Y')] Runnig '$invocation'" >> $logFile

$invocation >> $logFile 2>&1 &
pid=$!

echo $pid > $pidFile
echo "[$(date +'%H:%M:%S %d/%m/%Y')] PID: $pid" >> $logFile

wait $pid
code=$?
echo "[$(date +'%H:%M:%S %d/%m/%Y')] EXIT CODE: $code" >> $logFile
echo "" >> $logFile;
echo "" >> $logFile;

rm $pidFile > /dev/null 2>&1
exit $code
