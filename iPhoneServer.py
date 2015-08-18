from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor
import shelve
import socket

class IphoneChat(Protocol):
	def __init__(self):
		self.d = shelve.open("stateStorage")
		try:
			print "Current state: " + self.d['state']
		except:
			self.d['state'] = "Off"

	def connectionMade(self):
		#self.transport.write("""connected""")
		self.factory.clients.append(self)
		print "clients are ", self.factory.clients
	
	def connectionLost(self, reason):
		self.d.close()
		self.factory.clients.remove(self)


	def dataReceived(self, data):
	    #print "data is ", data
		a = data.split(':')
		if len(a) > 1:
			command = a[0]
			content = a[1]
			
			msg = ""
			if command == "iam":
				self.name = content
				msg = "state = " + self.d['state']
				
			elif command == "msg":
				msg = "state = " + content
				print "Changed state in db from: " + self.d['state'] + " to: " + content
				self.d['state'] = content
				UDP_IP = "127.0.0.1"
				UDP_PORT = 9930
				MESSAGE = "Cmd : Light = " + content + " ;"
				print "UDP target IP:", UDP_IP
				print "UDP target port:", UDP_PORT
				print "message:", MESSAGE
				sock = socket.socket(socket.AF_INET, # Internet
				                     socket.SOCK_DGRAM) # UDP
				sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))

			print msg

			for c in self.factory.clients:
				c.message(msg)

	def message(self, message):
		self.transport.write(message + '\n')

state = 0

factory = Factory()
factory.protocol = IphoneChat
factory.clients = []

reactor.listenTCP(9930, factory)
print "Iphone Chat server started"
reactor.run()

