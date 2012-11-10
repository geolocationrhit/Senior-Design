#!/usr/bin/python

import socket
import sys
from ctypes import *
import re
import select

class WP(Structure):
	_fields_ = [("x",c_double),("y",c_double)]

def newMovementCommand(command, option):
	return "<MOVCMD>{0} {1}</MOVCMD>\n".format(command,option);

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
#s.send(newMovementCommand("FWD", 2*pow(10,6)))
while True:
	sel = raw_input("Selection: \r\n1: FWD\r\n2: BCK\r\n3: TURN\r\n4: COMPASS QUERY\r\n5: GPS QUERY\r\n9: EXIT\r\n")
	if sel == 'EXIT' or sel == '9':
		print "Sending Exit"	
		s.send("Exit")
		break
	if sel == 'FWD' or sel == '1':
		opt = raw_input("\r\nOption: ")
		cmd = newMovementCommand("FWD", int(float(opt)*pow(10,6)))
		print "Sending command " + cmd	
		s.send(cmd)
	elif sel == 'BACK' or sel == '2':
		opt = raw_input("\r\nOption: ")
		cmd = newMovementCommand("BACK", int(float(opt)*pow(10,6)))
		print "Sending command " + cmd	
		s.send(cmd)
	elif sel == 'TURN' or sel == '3':
		opt = raw_input("\r\nOption: ")
		cmd = newMovementCommand("TURN", float(opt))
		print "Sending command " + cmd	
		s.send(cmd)
	elif sel == 'COMPASS Q' or sel == '4':
		cmd = newMovementCommand("COMPASSQ", 0)
		print "Sending command " + cmd	
		s.send(cmd)	
	elif sel == 'GPS Q' or sel == '5':
		cmd = newMovementCommand("GPSQ", 0)
		print "Sending command " + cmd	
		s.send(cmd)		
	else:
		print "Command not recognized"
		continue
	ready = select.select([s],[],[], 10)
	if ready[0]:
		data = s.recv(BUFFER_SIZE)
	#while data == "":
	#	data = s.recv(BUFFER_SIZE)
	print "received data: ", data
	print "\r\n"


s.close()
