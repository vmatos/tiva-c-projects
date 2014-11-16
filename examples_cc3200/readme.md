


# To run the FTDI driver:

   modprobe ftdi-sio
   echo 0451 c32a > /sys/bus/usb-serial/drivers/ftdi_sio/new_id
  
Or you can use this udev rule (figured this out for energia on ubuntu 14.04), 
  ATTRS{idVendor}=="0451", ATTRS{idProduct}=="c32a", MODE="0660", GROUP="dialout", RUN+="/sbin/modprobe ftdi-sio", RUN+="/bin/sh -c '/bin/echo 0451 c32a > /sys/bus/usb-serial/drivers/ftdi_sio/new_id'"
  
  cc3200-sdk folder goes into ../


# References:
https://hackpad.com/Using-the-CC3200-Launchpad-Under-Linux-Rrol11xo7NQ
