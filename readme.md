### Uses the TivaWare library from TI:
SW-TM4C: TivaWare for C Series Software (Complete), 2.1.2.111

## How to set up the system:
Needed:

1. ARM cross compiler (arm-none-eabi-***)
2. TI's Tivaware
3. OpenOCD or 'https://github.com/utzig/lm4tools to flash the device'

Check this and investigate this further: https://github.com/szczys/tiva-c-launchpad-template
'TI has a restrictive license on all of their example files. Becuase of this you cannot use their Makefiles, Linker scripts, or Startup code in open source repositories.
This template gets around this issue by creating symbolic links to these files. You need the TivaWare package for libraries anyway, this simply creates links to the pertinent files at compile time.'

Some straight instructions: `http://www.strainu.ro/programming/embedded/programming-for-the-tiva-c-launchpad-on-linux/`

1. Get ARM GCC toolchain:
`https://launchpad.net/~team-gcc-arm-embedded`

```
   sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
   sudo apt-get update
   sudo apt-get install gcc-arm-embedded
```

2. TivaWare:
Google SW-TM4C: TivaWare for C Series Software

```
   mkdir TivaWare
   unzip SW-TM4C-2.#.#.####.exe
   make
```
Directory structure:

```
.
|-> tiva-c-projects
|-> TivaWare
```

3. So far I'm using lm4tools, not OpenOCD

```
	git clone https://github.com/utzig/lm4tools.git
	cd lm4tools/lm4ﬂash
	make
	sudo cp lm4flash /usr/local/bin/

	echo 'ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", GROUP="users", MODE="0660"' | \
	sudo tee /etc/udev/rules.d/99-stellaris-launchpad.rules
```

# TODO:

* Better instructions
* Descriptions for each individual project

# Important!!!!
Tivaware BUG regarding USB ring buffer.
Fix on file: usblib/usbbuffer.c

```
    USBBufferInfoGet(const tUSBBuffer *psBuffer, tUSBRingBufObject *psRingBuf)
    (...)
    -    psRingBuf->ui32Size = psBufVars->sRingBuf.ui32ReadIndex;
    +    psRingBuf->ui32Size = psBufVars->sRingBuf.ui32Size;
```
