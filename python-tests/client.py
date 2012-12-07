#!/usr/bin/python

import socket
import time
import sys

if len(sys.argv) == 3:
	host = sys.argv[1]
	port = int(sys.argv[2])
else:
	host = "localhost"
	port = 5006

while True:
	time.sleep(2)
	s = socket.socket()
	s.connect((host,port))
	s.send('DataRequest')
	data = s.recv(1024)
	print "Received: {0}".format(data)
	s.close

