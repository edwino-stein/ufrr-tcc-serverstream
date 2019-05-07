<?php

if(count($argv) < 2){
    echo "ERROR: Reference file missing\n";
    exit(1);
}

$file = $argv[1];
echo "Reference File: ", $file, "\n";

if(($key = ftok("README.md", "G")) == -1){
    echo "ERROR: ";
    die("ftok");
}

echo "Queue key: ",$key, "\n";

if(!msg_queue_exists($key)){
    echo "ERROR: ";
    die("message queue doesn't exists");
}

/* Connect to message queue */
if(($msqid = msg_get_queue($key)) === FALSE){
    echo "ERROR: ";
    die("msg_get_queue");
}

$cmd  = [
    'cmd' => 'stop',
    'extra' => []
];

echo "JSON: ", json_encode($cmd, JSON_UNESCAPED_SLASHES | JSON_FORCE_OBJECT | JSON_PRETTY_PRINT), "\n";

$json = json_encode($cmd, JSON_UNESCAPED_SLASHES | JSON_FORCE_OBJECT)."\0" ;
echo "Message length: ", strlen($json), " bytes\n";

echo "Sending text to msg queue.\n";
if(!msg_send($msqid, 12, $json, false)){
    echo "ERROR: ";
    die("msg_send");
}

echo "Done\n";
