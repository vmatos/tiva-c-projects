#!/usr/bin/env python
import socket

ip = '192.168.1.80'
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print "Connecting to: ", ip
s.connect((ip, 8080))

s.send("Knock Knock")
print s.recv(1024)
s.close()
