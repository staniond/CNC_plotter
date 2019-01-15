#!/bin/bash
while :
do
	socat pty,link=$HOME/dev/ttyUSB0,waitslave tcp:192.168.0.36:8888
done

