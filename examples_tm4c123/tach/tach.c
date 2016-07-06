#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"

#include "ringbuf.h"
#include "utils.h"

#define M_PI 3.14159265358979323846

//*****************************************************************************
//
// The system tick rate expressed both as ticks per second and a millisecond
// period.
//
//*****************************************************************************
#define SYSTICKS_PER_SECOND 100
#define SYSTICK_PERIOD_MS (1000 / SYSTICKS_PER_SECOND)

//*****************************************************************************
//
// Global system tick counter
//
//*****************************************************************************
volatile uint32_t g_ui32SysTickCount = 0;

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

#define READINGS_SIZE 5
static uint32_t g_ui32SysClock = 0;
static tRingBufObject g_tReadings;
static uint32_t g_ui32ReadingsBuf[READINGS_SIZE];


void ConfigureUART(uint32_t baud)
{
  // For debugging
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  // Enable UART0
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  // Configure GPIO Pins for UART mode.
  MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
  MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
  MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  // Use the internal 16MHz oscillator as the UART clock source.
  UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
  // Initialize the UART for console I/O.
  UARTStdioConfig(0, baud, 16000000);
}

void WT0CCP0_ISR(void)
{
  uint32_t ui32IntStatus = MAP_TimerIntStatus(WTIMER0_BASE, 0);
  MAP_TimerIntClear(WTIMER0_BASE, ui32IntStatus);
  
  if (ui32IntStatus & TIMER_CAPA_EVENT)
  {
    MAP_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2^MAP_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2));
    
    // Store 
    if (RingBufFull(&g_tReadings))
    {
      RingBufReadOne(&g_tReadings);
    }
    RingBufWriteOne(&g_tReadings, 0xFFFFFFFF - MAP_TimerValueGet(WTIMER0_BASE, TIMER_A));
    
    MAP_TimerLoadSet(WTIMER0_BASE, TIMER_A, 0xFFFFFFFF);
  }
  else
  {
    UARTprintf("Not CAPA\n");
  }
  
}

//*****************************************************************************
//
// Interrupt handler for the system tick counter.
//
//*****************************************************************************
void SysTickIntHandler(void)
{
  // Update our system time.
  g_ui32SysTickCount++;
  
  if (g_ui32SysTickCount >= 10)
  {
    g_ui32SysTickCount = 0;
    
    if (!RingBufFull(&g_tReadings))
    {
      UARTprintf("Not enough readings\n");
    }
    else
    {
      uint64_t ui64ReadingsSum = 0;
      for(int i=0; i<g_tReadings.ui32Size; i++)
      {
        ui64ReadingsSum += g_tReadings.pui32Buf[i];
      }
      ui64ReadingsSum /= g_tReadings.ui32Size;
      float fElapsedTime = ui64ReadingsSum/((float)g_ui32SysClock);
      float fAngVel = (2*M_PI)/fElapsedTime;
      
      char str_float[30];
      ftoa(fElapsedTime, str_float);
      UARTprintf("Avrg T: %s, ", str_float);
      ftoa(1/fElapsedTime, str_float);
      UARTprintf("freq: %s, ", str_float);
      ftoa(fAngVel, str_float);
      UARTprintf("ang: %s\n", str_float);
    }
  }
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    MAP_FPUEnable();
    MAP_FPULazyStackingEnable();

    // Set the clocking to run directly from the crystal.
    // Running at 50MHz
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                     SYSCTL_OSC_MAIN);
                     
    g_ui32SysClock = MAP_SysCtlClockGet();

    // Enable the GPIO port that is used for the on-board LED.
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the GPIO pins for the LED (PF2).
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    
    ConfigureUART(115200);
    
    RingBufInit(&g_tReadings, g_ui32ReadingsBuf, READINGS_SIZE);
    
    // Enable input for WTimer0
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    // Configure the CCP pin as input
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
    MAP_GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
    MAP_GPIOPinConfigure(GPIO_PC4_WT0CCP0);
    // Enable wide timer 0
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    
    // Configure Timers on Capture Edge Time mode
    // WTimer 0
    MAP_TimerConfigure(WTIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME);
    MAP_TimerClockSourceSet(WTIMER0_BASE, TIMER_CLOCK_SYSTEM); // 50Mhz
    MAP_TimerControlEvent(WTIMER0_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);
    // 	Register handler, instead of putting interrupt handler in startupgcc, and then
    // enable.
    TimerIntRegister(WTIMER0_BASE, TIMER_A, WT0CCP0_ISR);
    // Enable Interrupts
    MAP_TimerIntEnable(WTIMER0_BASE, TIMER_CAPA_EVENT);
    // Enable the timers
    MAP_TimerEnable(WTIMER0_BASE, TIMER_A);
    
    // Enable the system tick.   
    MAP_SysTickPeriodSet(MAP_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    MAP_SysTickIntEnable();
    MAP_SysTickEnable();

    // Enable processor interrupts.
    MAP_IntMasterEnable();

    UARTprintf("Tachometer running at: %d Mhz\n", g_ui32SysClock);

    // Loop forever echoing data through the UART.
    while(1)
    {
    }
}
