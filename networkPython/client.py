#!/usr/bin/python

import socket
import sys

def gpsMessage():
	# grab latest GPS data
	return "<GPS>12.1029102 59.192910</GPS>\n"

def antennaMessage():
	# grab latest antenna data
	return "<RSS>-12.2910</RSS>\n<AOA>49.192012</AOA>\n"

#def parseMovementCommand(movcmd):

#def parseRecMobile(command):

if len(sys.argv) == 3:
	TCP_IP = sys.argv[1]
	TCP_PORT = int(sys.argv[2])
else:
	TCP_IP = "127.0.0.1"
	TCP_PORT = 5006

BUFFER_SIZE = 1024
MESSAGE = "Hello, World!\n"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print "Connecting to " + TCP_IP + " on port " + str(TCP_PORT)
s.connect((TCP_IP, TCP_PORT))
#s.send(MESSAGE)
s.send(gpsMessage() + antennaMessage())
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:", data
