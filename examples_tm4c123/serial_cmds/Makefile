#******************************************************************************
#
# Makefile - Rules for building the uart_echo example.
#
# Copyright (c) 2012-2014 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
# Texas Instruments (TI) is supplying this software for use solely and
# exclusively on TI's microcontroller products. The software is owned by
# TI and/or its suppliers, and is protected under applicable copyright
# laws. You may not combine this software with "viral" open-source
# software in order to form a larger program.
# 
# THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
# NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
# NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
# CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
# DAMAGES, FOR ANY REASON WHATSOEVER.
# 
# This is part of revision 2.1.0.12573 of the EK-TM4C123GXL Firmware Package.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM

#
# The base directory for TivaWare.
#
ROOT=../..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find header files that do not live in the source directory.
#
TIVAWARE=${ROOT}/../TivaWare
IPATH=${TIVAWARE}


#
# The default rule, which causes the program to be built.
#
all: ${COMPILER}
all: ${COMPILER}/serial_cmds.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
	
#
# The rule to flash
#
flash:
	@lm4flash ${COMPILER}/serial_cmds.bin

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the example.
#
${COMPILER}/serial_cmds.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/serial_cmds.axf: ${COMPILER}/main.o
${COMPILER}/serial_cmds.axf: ${COMPILER}/uart_protocol.o
${COMPILER}/serial_cmds.axf: ${TIVAWARE}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/serial_cmds.axf: ${TIVAWARE}/utils/ringbuf.o
${COMPILER}/serial_cmds.axf: ${TIVAWARE}/utils/uartstdio.o
${COMPILER}/serial_cmds.axf: TM4C123GH6PM.ld
SCATTERgcc_serial_cmds=TM4C123GH6PM.ld
ENTRY_serial_cmds=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C123_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
