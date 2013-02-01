#!/usr/bin/python

import socket
import sys
from ctypes import *
import re

if len(sys.argv) == 3:
	TCP_IP = sys.argv[1]
	TCP_PORT = int(sys.argv[2])
else:
	TCP_IP = "127.0.0.1"
	TCP_PORT = 5005

BUFFER_SIZE = 1024
MESSAGE = "Hello, World!\n"

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print "Connecting to " + TCP_IP + " on port " + str(TCP_PORT)
s.connect((TCP_IP, TCP_PORT))
s.send(TCP_IP + "\n")
#s.send(gpsMessage() + antennaMessage())
#data = s.recv(BUFFER_SIZE)
#print "received data:", data
#parseCommand(data)
s.close()
