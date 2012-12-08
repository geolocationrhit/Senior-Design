import SocketServer
import socket
import sys
from ctypes import *

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

def newDataMessage():
	#wayPoint = computeNewLocation(0)
	#return "<NWP>{0} {1}</NWP>\n".format(wayPoint[0],wayPoint[1])
	# Get RSS data from the SDR over ethernet
	s = socket.socket()
	s.connect(('192.168.10.2',5005))
	s.send('DataRequest')
	SDRdata = s.recv(1024)
	print "Received from SDR: {0}".format(SDRdata)
	s.close

	# get GPS data from the GPS
	gpsPoint = movementLib.getGPSlocation()
	#return "GPS query: long: {0}, lat: {1}, valid: {2}".format(gpsPoint.x,gpsPoint.y,gpsPoint.valid)
	
	return "LAT:{0} LONG:{1} TIME:{2} HEADING:{3} RSS:{4}\n".format(gpsPoint.y,gpsPoint.x,gpsPoint.time,movementLib.getCompassHeading(),SDRdata[4:])

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
		self.request.sendall(newDataMessage())

if __name__ == "__main__":
	if len(sys.argv) == 3:
		#print sys.argv
		HOST = sys.argv[1]
		PORT = int(sys.argv[2])
	else:
		HOST = "localhost"
		PORT = 5006

	initLibs()
	movementLib.initSensors()

	# Create the server, binding to localhost on port 9999
	print "Setting up a server at " + HOST + " on port " + str(PORT)
	server = SocketServer.TCPServer((HOST, PORT), MyTCPHandler)

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
