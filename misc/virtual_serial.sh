#!/bin/bash
while :
do
	socat pty,link=./ttyUSB0,waitslave tcp:192.168.0.36:8888
done

