Small working example of a command parser through serial port.
Frame packet layout:

-----------------------------------------------------------------------------------
| Start frame | control byte | payload len |    data    |   crc16 (8005 poly)     |
-----------------------------------------------------------------------------------
| 8bit | 8bit |     8bit     |    8bit     | 8bit x len | 8bit (msB) | 8bit (lsB) |
-----------------------------------------------------------------------------------
| 0x7E | 0x7E |     ----     |    0-255    | 8bit x len | 8bit (msB) | 8bit (lsB) |
-----------------------------------------------------------------------------------



ToDo:
- What if 0x7E in payload? Do: http://en.wikipedia.org/wiki/High-Level_Data_Link_Control#Asynchronous_framing
