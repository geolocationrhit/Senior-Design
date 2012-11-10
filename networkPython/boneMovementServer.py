import SocketServer
import sys
from ctypes import *
import re

def initCompass():
	# load the shared object
	compassLib = cdll.LoadLibrary('../sharedLibs/compassLib.so') # make these libs global and initialize all at once
	return 0

def initLibs():
	global movementLib
	global waypointLib
	waypointLib = cdll.LoadLibrary('../sharedLibs/waypointLib.so')
	movementLib = cdll.LoadLibrary('../sharedLibs/movementLib.so')
	#movementLib.getCompassHeading.argtypes = 
	movementLib.turn.argtypes = [c_float]
	movementLib.turn.restypes = [c_int]
	movementLib.moveForward.argtypes = [c_int]
	movementLib.getCompassHeading.restype = c_float

	movementLib.getGPSlocation.restype = dataGPS
	#movementLib.getCompassHeading.argtypes = [c_void]
class dataGPS(Structure):
	_fields_ = [("x",c_double),("y",c_double),("time",c_double),("valid",c_int)]

class WP(Structure):
	_fields_ = [("x",c_double),("y",c_double)]

def parseWP(command):
	m = re.match('<NWP>([-+]?[0-9]*\.?[0-9]+) ([-+]?[0-9]*\.?[0-9]+)</NWP>', command)
	print "Adding waypoint: {0} {1}".format(m.group(1),m.group(2))
	#waypointLib = cdll.LoadLibrary('../sharedLibs/waypointLib.so') # this is loaded every time, so I bet the waypoints don't save
	waypointLib.addWaypointxy.argtypes = [c_double,c_double]
	waypointLib.addWaypoint.argtypes = [POINTER(WP)]
	waypointLib.getCurrentWaypoint.restype = POINTER(WP)
	waypointLib.addWaypointxy(float(m.group(1)),float(m.group(2)))
	w = waypointLib.getCurrentWaypoint()[0]; # getCurrentWaypoint returns a pointer, so we need to dereference with an array access [0]
	print "The waypoint in the queue was, x: {0} y: {1}".format(w.x,w.y)

def parseMOVCMD(command):
	m = re.match('<MOVCMD>(FWD|BACK|TURN|COMPASSQ|GPSQ) ([-+]?[0-9]*\.?[0-9]+)</MOVCMD>', command)
	operation = m.group(1);
	option = m.group(2);
	if operation == "FWD":
		print "Forward command received: " + option
		movementLib.moveForward(int(option))
		return ""
	elif operation == "BACK":
		print "Back command received, but it's not implemented currently"
		return ""
	elif operation == "TURN":
		print "Turn command received: " + option
		movementLib.turn(float(option))
		return ""
	elif operation == "COMPASSQ": # move this to a different parser...it's not a movcmd
		print "Compass query received"
		print movementLib.getCompassHeading()
		return "Compass query: " + str(movementLib.getCompassHeading()) + " degrees"
	elif operation == "GPSQ":
		print "GPS query received"
		gpsPoint = movementLib.getGPSlocation()
		return "GPS query: long: {0}, lat: {1}, valid: {2}".format(gpsPoint.x,gpsPoint.y,gpsPoint.valid)
	else:
		print "No command received"
		return "Not a valid movement command"

def parseCommand(command):
	comType = command[command.find("<")+1:command.find(">")]
	if comType == "NWP":
		return parseWP(command)
	elif comType == "MOVCMD":
		return parseMOVCMD(command)
	else:
		print "Unknown command: " + comType
		return "Unknown command"

def newMovementCommand():
	return "<MOVCMD>FWD 1.0 RIGHT 0.5</MOVCMD>\n"

class MyTCPHandler(SocketServer.BaseRequestHandler):
	"""
	The RequestHandler class for our server.

	It is instantiated once per connection to the server, and must
	override the handle() method to implement communication to the
	client.
	"""

	def handle(self):
		# self.request is the TCP socket connected to the client
		self.data = self.request.recv(1024).strip()
		print "{} wrote:".format(self.client_address[0])
		print self.data
		# just send back the same data, but upper-cased
		cmdret = parseCommand(self.data)
		if cmdret == "":
			self.request.sendall("Successfully executed command")
		else:
			self.request.sendall(cmdret);
		while(self.data != "Exit"):
			self.data = self.request.recv(1024).strip()
			print self.data
			cmdret = parseCommand(self.data)
			if cmdret == "":
				self.request.sendall("Successfully executed command")
			else:
				self.request.sendall(cmdret);

if __name__ == "__main__":
	if len(sys.argv) == 3:
		#print sys.argv
		HOST = sys.argv[1]
		PORT = int(sys.argv[2])
	else:
		HOST = "localhost"
		PORT = 5006

	# Create the server, binding to localhost on port 9999
	print "Setting up a server at " + HOST + " on port " + str(PORT)
	server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

	initLibs()
	movementLib.initSensors()
	# Activate the server; this will keep running until you
	# interrupt the program with Ctrl-C
	try:
		server.serve_forever(0.5)
	except KeyboardInterrupt:
		print "\nShutting down the server"
		server.shutdown()

"""
#!/usr/bin/python

import socket


TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print 'Connection address:', addr
while 1:
    data = conn.recv(BUFFER_SIZE)
    if not data: break
    print "received data:", data
    conn.send(data)  # echo
conn.close()
"""
