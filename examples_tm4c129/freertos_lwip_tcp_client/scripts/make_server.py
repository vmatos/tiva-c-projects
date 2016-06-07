#!/usr/bin/env python
import socket

#create an INET, STREAMing socket
serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#bind the socket to a public host,
# and a well-known port
serversocket.bind(('', 8080))
#become a server socket
serversocket.listen(5)

while 1:
    #accept connections from outside
    (clientsocket, address) = serversocket.accept()
    print "socket ", clientsocket
    print "Ip", address
    
    print "Got: ", clientsocket.recv(1024)
    clientsocket.send("Who's there?")
