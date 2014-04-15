#******************************************************************************
#
# Makefile - Rules for building the example.
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
LIB_NAME = libenc28j60
TEST_NAME = enc28j60_blink_test

# VERBOSE = 1
UART_STDIO = -DUART_STDIO

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM

#
# The base directory for TivaWare.
#
TIVAWARE=../TivaWare

#
# The source directory.
#
VPATH = src test
LIB_SRC = spi_drv.c enc28j60_drv.c
LIB_OBJ = ${addprefix ${COMPILER}/,${LIB_SRC:%.c=%.o}}
TEST_SRC = enc28j60_blink_test.c startup_gcc.c
TEST_OBJ = ${addprefix ${COMPILER}/,${TEST_SRC:%.c=%.o}}

#
# The lib directory.
#
LIB_DEST = lib

#
# Where to find header files that do not live in the source directory.
#
IPATH=${TIVAWARE} include

#
# Include the common make definitions.
#
include makedefs

#
# The default rule, which causes the example to be built.
#
all: ${COMPILER}
all: install

install: ${COMPILER}/${LIB_NAME}.a
	@cp ${COMPILER}/${LIB_NAME}.a ${LIB_DEST}/${LIB_NAME}.a
	
test: all
test: ${COMPILER}/enc28j60_blink_test.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
	@rm -rf ${LIB_DEST} ${wildcard *~}
	
#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}
	@mkdir -p ${LIB_DEST}
	
#
# Rule to investigate the produced library
#
dump:

#
# Rules for building the library
#
${COMPILER}/${LIB_NAME}.a: ${LIB_OBJ}

#
# Rules for compiling blinky test
#
${COMPILER}/${TEST_NAME}.axf: ${TEST_OBJ}
${COMPILER}/${TEST_NAME}.axf: lib/${LIB_NAME}.a
${COMPILER}/${TEST_NAME}.axf: ${TIVAWARE}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/${TEST_NAME}.axf: ${TIVAWARE}/utils/${COMPILER}/libutils.a
${COMPILER}/${TEST_NAME}.axf: TM4C123GH6PM.ld
SCATTERgcc_${TEST_NAME}=TM4C123GH6PM.ld
ENTRY_${TEST_NAME}=ResetISR

#
# The rule to flash
#
flash: test
	@lm4flash ${COMPILER}/enc28j60_blink_test.bin

CFLAGSgcc=-DTARGET_IS_TM4C123_RB1 ${UART_STDIO}

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif





