SD card using FAT file system

This example application demonstrates reading a file system from
an SD card.  It makes use of FatFs, a FAT file system driver.  It
provides UART-based command line for viewing and navigating the 
file system on the SD card.

For additional details about FatFs, see the following site:
http://elm-chan.org/fsw/ff/00index_e.html

he RS232 communication parameters should be set to 115,200 bits
per second, and 8-n-1 mode.  When the program is started a message will
be printed to the terminal.  Type ``help'' for command help.
