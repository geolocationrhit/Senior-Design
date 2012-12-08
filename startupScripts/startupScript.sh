#!/bin/sh

ifconfig eth0 up
ifconfig eth0 192.168.10.1
python /home/ubuntu/Senior-Design/MATLABcode/server.py `ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'` 5005 &

exit 0
