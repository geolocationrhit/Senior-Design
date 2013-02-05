#!/bin/sh

ifconfig eth0 up
ifconfig eth0 192.168.10.1
sleep 10
python /home/ubuntu/Senior-Design/BeagleBoneCode/dataServer.py `ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'` 5005 &
python /home/ubuntu/Senior-Design/BeagleBoneCode/registerWithCP.py 192.168.2.50 5007 `ifconfig wlan0 | grep 'inet addr:' | cut -d: -f2 | awk '{ print $1}'`

exit 0
