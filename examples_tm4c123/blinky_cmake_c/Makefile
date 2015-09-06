#******************************************************************************
#
# Makefile - Rules for building the 
#
#******************************************************************************
PRJ_NAME=blinky

#
# Defines the part type that this project uses.
#
PART=TM4C123GH6PM
VERBOSE=1

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
# The default rule, which causes the blinky example to be built.
#
all: ${COMPILER}
all: ${COMPILER}/${PRJ_NAME}.axf

#
# The rule to flash
#
flash:
	@lm4flash ${COMPILER}/${PRJ_NAME}.bin

#
# The rule to clean out all the build products.
#
clean:
	@rm -rf ${COMPILER} ${wildcard *~}

#
# The rule to create the target directory.
#
${COMPILER}:
	@mkdir -p ${COMPILER}

#
# Rules for building the blinky example.
#
${COMPILER}/${PRJ_NAME}.axf: ${COMPILER}/startup_${COMPILER}.o
${COMPILER}/${PRJ_NAME}.axf: ${COMPILER}/blinky.o
${COMPILER}/${PRJ_NAME}.axf: tm4c123g.ld
SCATTERgcc_${PRJ_NAME}=tm4c123g.ld
ENTRY_${PRJ_NAME}=ResetISR
CFLAGSgcc=-DTARGET_IS_TM4C123_RB1

#
# Include the automatically generated dependency files.
#
ifneq (${MAKECMDGOALS},clean)
-include ${wildcard ${COMPILER}/*.d} __dummy__
endif
