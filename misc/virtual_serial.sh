#!/bin/bash
ADDRESS=${1:-192.168.0.36:8888}
FILE=${2:-./ttyUSB0}
echo Connecting $ADDRESS with $FILE
while :
do
	socat pty,link=$FILE,waitslave tcp:$ADDRESS
done

