# http://www.cmake.org/Wiki/CMake_Cross_Compiling

# Set cross compilation information
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(TIVA_WARE_PATH "/home/hazard/tivaC/TivaWare" CACHE STRING "TivaWare path")

# GCC toolchain prefix
set(TOOLCHAIN_PREFIX "arm-none-eabi")

SET(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
SET(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
SET(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)

# Processor specific definitions
add_definitions(-DTARGET_IS_TM4C123_RA1)
add_definitions(-Dgcc)
