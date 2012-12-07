#!/usr/bin/python

import socket
import sys
from ctypes import *
import re

class WP(Structure):
	_fields_ = [("x",c_double),("y",c_double)]

def gpsMessage():
	# grab latest GPS data
	return "<GPS>12.1029102 59.192910</GPS>\n"

def antennaMessage():
	# grab latest antenna data
	return "<RSS>-12.2910</RSS>\n<AOA>49.192012</AOA>\n"

def initCompass():
	# load the shared object
	compassLib = cdll.LoadLibrary('../sharedLibs/compassLib.so')
	return 0

def parseWP(command):
	m = re.match('<NWP>([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+)</NWP>', command)
	print "Adding waypoint: {0} {1}".format(m.group(1),m.group(2))
	waypointLib = cdll.LoadLibrary('../sharedLibs/waypointLib.so')
	waypointLib.addWaypointxy.argtypes = [c_double,c_double]
	waypointLib.addWaypoint.argtypes = [POINTER(WP)]
	waypointLib.getCurrentWaypoint.restype = POINTER(WP)
	waypointLib.addWaypointxy(float(m.group(1)),float(m.group(2)))
	w = waypointLib.getCurrentWaypoint()[0]; # getCurrentWaypoint returns a pointer, so we need to dereference with an array access [0]
	print "The waypoint in the queue was, x: {0} y: {1}".format(w.x,w.y)

def parseCommand(command):
	comType = command[command.find("<")+1:command.find(">")]
	if comType == "NWP":
		parseWP(command)
	else:
		print "Unknown command: " + comType

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
print "received data:", data
parseCommand(data)
s.close()
