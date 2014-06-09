#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
//#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/pwm.h"


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//*****************************************************************************
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

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
	uint32_t ui32_Period, ui32_PWM=0, ui32_PWM_step = 40, ui32_PWM_min=2000, ui32_PWM_max=4000;
	uint32_t ui32_sw1;
    uint32_t ui32_sw2;
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    //MAP_FPUEnable();
    //MAP_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable the GPIO pins for the LED (PF2).
    //
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    
    // Configure UART
    InitConsole();

    
    // -----------------------------------------------------------------
    // Configure PWM for servo control
    // -----------------------------------------------------------------
    // follow page 1240 of the datasheet:
    ui32_Period = (MAP_SysCtlClockGet()/8) / 50; //PWM frequency 50HZ, T=20ms
    // GOTO middle of servo in the beginning
    ui32_PWM = ui32_PWM_min + (ui32_PWM_max - ui32_PWM_min) / 2;
    ui32_PWM_max = ui32_Period*.1;
    ui32_PWM_min = ui32_Period*.05;
    
    //Configure PWM Clock to match system
	MAP_SysCtlPWMClockSet(SYSCTL_PWMDIV_8);
    
    // Enable system clock for PWM0 module
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  
    // Enable the GPIO port that is used for the PWM outputs
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    
    //Configure PB5,PB5 Pins as PWM
    MAP_GPIOPinConfigure(GPIO_PB4_M0PWM2);
    MAP_GPIOPinConfigure(GPIO_PB5_M0PWM3);
    MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    MAP_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    
    //Configure PWM Options
    MAP_PWMGenConfigure(PWM0_BASE, PWM_GEN_1 , PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    //
    // Set the PWM period to 50Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk.  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 50Hz) * 16MHz/8div = 40000 cycles.  Note that
    // the maximum period you can set is 2^16 - 1.
    // TODO: modify this calculation to use the clock frequency that you are
    // using.
    //
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ui32_Period);
    
    //Set PWM duty - 25% and 75%
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ui32_PWM);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ui32_PWM);
    
    // Enable the PWM generator
    MAP_PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    // Turn on the Output pins
    MAP_PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, true);
    
    
    // -----------------------------------------------------------------
    // Enable PORTF and Configure SW1 and SW2 as input
    // -----------------------------------------------------------------
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    MAP_GPIODirModeSet(GPIO_PORTF_BASE, (GPIO_PIN_4 | GPIO_PIN_0), GPIO_DIR_MODE_IN);

    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {
		// Get buttons:
		ui32_sw1 = MAP_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
		ui32_sw2 = MAP_GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);
		// if pressing SW1 on LaunchPad TM4C123G
		if( ui32_sw1 == 0 )
		{
			if(ui32_PWM >= ui32_PWM_min+ui32_PWM_step) {
				ui32_PWM -= ui32_PWM_step;
			}
		}
		if( ui32_sw2 == 0 )
		{
			if(ui32_PWM <= ui32_PWM_max-ui32_PWM_step) {
				ui32_PWM += ui32_PWM_step;
			}
		}
		
		UARTprintf("Period: %d, PWM: %d\n", ui32_Period, ui32_PWM);
		MAP_SysCtlDelay(2000000);
		
		MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2 , ui32_Period - ui32_PWM);
		MAP_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3 , ui32_PWM);
    }
}
