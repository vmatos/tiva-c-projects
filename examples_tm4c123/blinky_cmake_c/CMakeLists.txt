cmake_minimum_required(VERSION 2.8.3)
project(blinky_cmake_c)

# include(tm4c123g.cmake)

# http://www.cmake.org/Wiki/CMake_Cross_Compiling

# Set cross compilation information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# GCC toolchain prefix
set(TOOLCHAIN_PREFIX "arm-none-eabi")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-as)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}-ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}-objdump)

enable_language(ASM)

set(CPU "-mcpu=cortex-m4")
set(FPU "-mfpu=fpv4-sp-d16 -mfloat-abi=softfp")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mthumb ${CPU}  ${FPU} -MD")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mthumb ${CPU} ${FPU} -std=gnu99 -ffunction-sections -fdata-sections -MD -Wall -pedantic -Os -specs=tiva.specs")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthumb ${CPU} ${FPU} -ffunction-sections -fdata-sections -MD -Wall -pedantic -fno-exceptions -fno-rtti -Os -specs=tiva.specs")

#set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
#set(CMAKE_SHARED_LINKER_FLAGS "")

# Processor specific definitions
add_definitions(-DPART_TM4C123GH6PM)
add_definitions(-DTARGET_IS_TM4C123_RA1)
add_definitions(-Dgcc)

set(TIVA_WARE_PATH "/home/hazard/tivaC/TivaWare")
include_directories(${TIVA_WARE_PATH})

add_executable(${CMAKE_PROJECT_NAME}.bin startup_gcc.c blinky.c)
target_link_libraries(${CMAKE_PROJECT_NAME}.bin 
  ${TIVA_WARE_PATH}/usblib/gcc/libusb.a
  ${TIVA_WARE_PATH}/driverlib/gcc/libdriver.a
)


