#!/usr/bin/env python

from os import system
import serial
import sys
import crcmod


"""

"""
if __name__ == '__main__':
  port_str = "/dev/ttyACM0"

  try:
    ser = serial.Serial(port_str, 115200)   # open serial port that Arduino is using
  except:
    exit("Error: Could not open serial port: " + port_str)
    
  print ser                           # print serial config
  
  # ------------------------------------------------------------------------------
  str_crc  = "%c%c%c%c%c" % (0x7E, 0x7E, 0x00, 0x1, 0xDD) 
  
  
  crc16 = crcmod.mkCrcFun(0x18005, 0x0000, True)
  crc_result = crc16(str_crc)
  print "CRC:", "%x" % crc_result
  
  # print "%x,%x" % ( (crc_result&0xFF00)>>8 , crc_result&0xFF )
  
  # ------------------------------------------------------------------------------
  str_send = "%c%c%c%c%c%c%c" % ( 0x7E, 0x7E, 0x00, 0x1, 0xDD, (crc_result&0xFF00)>>8, crc_result&0xFF )
  
  print "Sent: ", str_send
  ser.write(str_send)
  
