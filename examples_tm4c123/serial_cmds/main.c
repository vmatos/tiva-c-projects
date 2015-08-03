//*****************************************************************************
//
// Example for parsing commands sent throught the serial port
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "uart_protocol.h"

//*****************************************************************************
//
//*****************************************************************************

//*****************************************************************************
// The error routine that is called if the driver library encounters an error.
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif


//*****************************************************************************
// This example demonstrates how to use serial command parser
//*****************************************************************************
int main(void)
{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    // Enable the GPIO port that is used for the on-board LED.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the GPIO pins for the LED (PF2).
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    // Enable the peripherals used by this example.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    
    // configure UART
    initUART();
    
    // Enable processor interrupts.
    ROM_IntMasterEnable();

    // Prompt for text to be entered.
    UARTSend((uint8_t *)"\033[2JEnter text: ", 16);

    // Loop forever echoing data through the UART.
    while(1)
    {
      //if(RingBufUsed(&g_tBuffRx) > 4) 
      //{
        //for(int i=4; i>0; i--) 
        //{
          //ROM_UARTCharPutNonBlocking(UART0_BASE,RingBufReadOne(&g_tBuffRx));
        //}
      //}
      UARTParse();
    }
}
