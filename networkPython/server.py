import SocketServer
import sys

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
		self.request.sendall("Hello\nThis is a test")

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
