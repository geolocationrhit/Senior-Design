#!/usr/bin/python

import socket
import os
import threading
import time

connections = list()
addresses = list()
length = len(connections)


def listener():
	while True:
		c,addr = s.accept()
		c.settimeout(2)
		connections.append(c)
		addresses.append(addr)
		length = len(connections)

def checkForHeartbeats():
	while True:
		bad_conns = list()
		length = len(connections)
		time.sleep(2)
		i = 0
		for i in range(0,length):
			if (connections[i] != None):
				try:
					print "Trying %d at {0}".format(addresses[i]) % (i)
					connections[i].send('Test')
				except:
					print "Connection at {0} has experienced an error... removing connection".format(addresses[i])
					bad_conns.append(i)
		bad_conns.reverse()
		for x in range(0,len(bad_conns)):
			del connections[bad_conns[x]]
			del addresses[bad_conns[x]]	

s = socket.socket()
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
host = socket.gethostname()
port = 27500
s.bind((host,port))
s.listen(5)
threading.Thread(target=listener).start()
threading.Thread(target=checkForHeartbeats).start()

