#******************************************************************************
#
# Makefile - Rules for building the FreeRTOS example.
#
# Copyright (c) 2013-2015 Texas Instruments Incorporated.  All rights reserved.
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
# This is part of revision 2.1.1.71 of the DK-TM4C129X Firmware Package.
#
#******************************************************************************

#
# Defines the part type that this project uses.
#
PART=TM4C1294NCPDT

#
# The base directory for TivaWare.
#
ROOT=../..

#
# Include the common make definitions.
#
include ${ROOT}/makedefs

#
# Where to find source files that do not live in this directory.
#
TIVAWARE=${ROOT}/../TivaWare

VPATH=../drivers
VPATH+=${TIVAWARE}/third_party/FreeRTOS/Source/portable/GCC/ARM_CM4F
VPATH+=${TIVAWARE}/third_party/FreeRTOS/Source/portable/MemMang
VPATH+=${TIVAWARE}/third_party/FreeRTOS/Source
VPATH+=${TIVAWARE}/third_party/lwip-1.4.1/apps/httpserver_raw
VPATH+=${TIVAWARE}/third_party/fatfs/port
VPATH+=${TIVAWARE}/third_party/fatfs/src
VPATH+=${TIVAWARE}/utils

#
# Where to find header files that do not live in the source directory.
#
IPATH=.
IPATH+=..
IPATH+=${TIVAWARE}
IPATH+=${TIVAWARE}/third_party/FreeRTOS/Source/portable/GCC/ARM_CM4F
IPATH+=${TIVAWARE}/third_party/FreeRTOS
IPATH+=${TIVAWARE}/third_party/lwip-1.4.1/apps
IPATH+=${TIVAWARE}/third_party/FreeRTOS/Source/include
IPATH+=${TIVAWARE}/third_party/lwip-1.4.1/ports/tiva-tm4c129/include
IPATH+=${TIVAWARE}/third_party/lwip-1.4.1/src/include
IPATH+=${TIVAWARE}/third_party/lwip-1.4.1/src/include/ipv4
IPATH+=${TIVAWARE}/third_party

#
# The default rule, which causes the FreeRTOS example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/freertos_lwip.axf

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}
	
#
# The rule to flash
#
flash: all
	@lm4flash ${COMPILER}/freertos_lwip.bin

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the FreeRTOS example.
#
# Application sources
${COMPILER}/freertos_lwip.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/freertos_lwip.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/idle_task.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/lwip_task.o
# Board specific on drivers
${COMPILER}/freertos_lwip.axf: ${COMPILER}/pinout.o
# FreeRTOS
${COMPILER}/freertos_lwip.axf: ${COMPILER}/port.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/heap_2.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/tasks.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/list.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/queue.o
#~ ${COMPILER}/freertos_lwip.axf: ${COMPILER}/timers.o
# lwIP
${COMPILER}/freertos_lwip.axf: ${COMPILER}/httpd.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/enet_fs.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/ff.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/mmc-ek-tm4c1294xl.o
# HW and other util libraries
${COMPILER}/freertos_lwip.axf: ${COMPILER}/lwiplib.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/locator.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/ustdlib.o
${COMPILER}/freertos_lwip.axf: ${COMPILER}/uartstdio.o
${COMPILER}/freertos_lwip.axf: ${TIVAWARE}/driverlib/${COMPILER}/libdriver.a
${COMPILER}/freertos_lwip.axf: freertos_lwip.ld
SCATTERgcc_freertos_lwip=freertos_lwip.ld
ENTRY_freertos_lwip=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C129_RA0 -DSNOWFLAKE -DEK_TM4C129_BP1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
