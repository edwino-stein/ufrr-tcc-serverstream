#!/bin/sh
logFile="/var/log/ffmpeg-daemon.log"
pidFile="/var/run/ffmpeg-daemon.pid"
options="-y -f v4l2 -framerate 25 -video_size 640x480 -i /dev/video0 -f mpegts -codec:v mpeg1video -s 640x480 -b:v 1000k -bf 0 tcp://0.0.0.0:8087?listen"
invocation="ffmpeg $options"
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
