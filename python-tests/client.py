#!/usr/bin/python

import socket

s = socket.socket()
host = socket.gethostname()
port = 27500

s.connect((host,port))
while True:
	print s.recv(1024)
s.close

