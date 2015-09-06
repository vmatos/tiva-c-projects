//*****************************************************************************
//
// blinky.cpp - Simple example to blink the on-board LED.
//
//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include <inc/tm4c1294ncpdt.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>

#include "pin.h"

#define delay(x)      SysCtlDelay(SysCtlClockGet() / 3 * x);

//*****************************************************************************
// Blink the on-board LED.
//*****************************************************************************
int main(void)
{
  volatile uint32_t ui32Loop;
  
  // Clock (80MHz)
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
  
  SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R12;

  GPIO_PORTN_DIR_R = 0x01;
  GPIO_PORTN_DEN_R = 0x01;

  while(1)
  {
    GPIO_PORTN_DATA_R |= 0x01;

    for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
    {
    }

    GPIO_PORTN_DATA_R &= ~(0x01);

    for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++)
    {
    }
  }
}
