FreeRTOS Example

This application utilizes FreeRTOS to perform a variety of tasks in a
concurrent fashion.  The following tasks are created:

* An lwIP task, which serves up web pages via the Ethernet interface.  This
  is actually two tasks, one which runs the lwIP stack and one which
  manages the Ethernet interface (sending and receiving raw packets).

* There is an automatically created idle task, which monitors changes in
  the board's IP address and sends those changes to the user via a UART
  message.

The finder application (in tools/finder) can also be used to discover the
IP address of the board.  The finder application will search the network
for all boards that respond to its requests and display information about
them.

For additional details on FreeRTOS, refer to the FreeRTOS web page at:
http://www.freertos.org/

For additional details on lwIP, refer to the lwIP web page at:
http://savannah.nongnu.org/projects/lwip/

-------------------------------------------------------------------------------

Copyright (c) 2009-2015 Texas Instruments Incorporated.  All rights reserved.
Software License Agreement

Texas Instruments (TI) is supplying this software for use solely and
exclusively on TI's microcontroller products. The software is owned by
TI and/or its suppliers, and is protected under applicable copyright
laws. You may not combine this software with "viral" open-source
software in order to form a larger program.

THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES, FOR ANY REASON WHATSOEVER.
