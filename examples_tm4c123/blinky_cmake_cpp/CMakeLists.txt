cmake_minimum_required(VERSION 2.8.3)
project(blinky_cmake_cpp)

include(tm4c123g.cmake)

set(TIVA_WARE_PATH "$ENV{HOME}/tivaC/TivaWare")
include_directories(${TIVA_WARE_PATH})

add_executable(${CMAKE_PROJECT_NAME}.axf startup_gcc.c blinky.cpp pin.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}.axf 
  ${TIVA_WARE_PATH}/usblib/gcc/libusb.a
  ${TIVA_WARE_PATH}/driverlib/gcc/libdriver.a
)

  
