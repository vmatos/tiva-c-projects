//*****************************************************************************
//
// spi_master.c - Example demonstrating how to configure SSI0 in SPI master
//                mode.
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#ifdef UART_STDIO
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#endif // UART_STDIO

#include "spi_drv.h"
#include "enc28j60_drv.h"

//*****************************************************************************
//
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - SSI0 peripheral
//! - GPIO Port A peripheral (for SSI0 pins)
//! - SSI0Clk - PA2
//! - SSI0Fss - PA3
//! - SSI0Rx  - PA4
//! - SSI0Tx  - PA5
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of SSI0.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//
//*****************************************************************************

//! 1- Write the address of the PHY register to write to into the MIREGADR register.
//! 2- Write the lower 8 bits of data to write into the MIWRL register.
//! 3- Write the upper 8 bits of data to write into the MIWRH register. 
//! Writing to this register auto-matically begins the MIIM transaction, 
//! so it must be written to after MIWRL. The MISTAT.BUSY bit becomes set.

static uint16_t ui16_byte[] = {
   ((0x40|0x1F)<<8)|0x02
  ,((0x40|0x14)<<8)|0x14
  ,((0x40|0x16)<<8)|0xA0
  ,((0x40|0x17)<<8)|0x0A
};

//*****************************************************************************
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//*****************************************************************************
#ifdef UART_STDIO
void
InitConsole(void)
{
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    MAP_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(0, 115200, 16000000);
}
#endif // UART_STDIO

int
main(void)
{
    uint32_t ui32_i;
    uint32_t ui32_reply[4];
    uint32_t ui32_sw1;
    uint32_t ui32_sw2;

    MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
#ifdef UART_STDIO
    InitConsole();
#endif // UART_STDIO

    //
    // Init SPI for enc28j60
    //
    enc28_InitSPI(1000000,SSI0_BASE); 
    
    //
    // Enable PORTF and Configure SW1 and SW2 as input
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    MAP_GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), GPIO_DIR_MODE_IN);
    
    UARTprintf("Press SW1 to set blinking.\n");
    UARTprintf("Press SW2 to Reset.\n"); 
    
    while(1)
    {
      ui32_sw1 = MAP_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
      ui32_sw2 = MAP_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
      
      // if pressing SW1 on LaunchPad TM4C123G, enc28j60 blink LEDs
      if( ui32_sw1 == 0 )
      {
        UARTprintf("Turn blinky LEDs on!\n");
        for(ui32_i=0;ui32_i<4;ui32_i++)
        {   
          ui32_reply[ui32_i] = (uint32_t) enc28_SPISend(ui16_byte[ui32_i]);
        }
        
        UARTprintf("Got reply: [ ");
        for(ui32_i=0;ui32_i<4;ui32_i++)
        {    
          UARTprintf("%02x ",ui32_reply[ui32_i]);
        }
        UARTprintf("]\n");
        
        MAP_SysCtlDelay(2000000);
      }
      
      // if pressing SW2 on LaunchPad TM4C123G, enc28j60 resets back
      if( ui32_sw2 == 0)  
      {
        UARTprintf("Reset!\n");
        enc28_Reset();
        MAP_SysCtlDelay(2000000);
      }
    }

    return(0);
}
