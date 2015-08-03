//*****************************************************************************
//
// blinky.cpp - Simple example to blink the on-board LED.
//
//*****************************************************************************
#include <stdbool.h>
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
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
  // Clock (80MHz)
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

  // Setup Red Led
  Pin led = Pin(PF1);
  led.setDir(OUT);

  while(1){
    led.toggle();
    delay(1);
  }
}
