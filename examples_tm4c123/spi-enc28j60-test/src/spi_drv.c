//*****************************************************************************
//
// spi_drv.c - SPI peripheral configuration
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
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

static uint32_t ui32_SSIx;

void enc28_InitSPI(uint32_t ui32_bitrate, uint32_t ui32_SSI_Base)
{
  uint32_t ui32_RxData;
  uint32_t ui32_SysCtl_Periph_SSI;
  uint32_t ui32_SysCtl_Periph_GPIO;
  
  ui32_SSIx = ui32_SSI_Base;
  
  switch(ui32_SSIx) {
    case SSI1_BASE:
      ui32_SysCtl_Periph_SSI = SYSCTL_PERIPH_SSI1;
      ui32_SysCtl_Periph_GPIO = SYSCTL_PERIPH_GPIOF;
      break;
    case SSI2_BASE:
      ui32_SysCtl_Periph_SSI = SYSCTL_PERIPH_SSI2;
      ui32_SysCtl_Periph_GPIO = SYSCTL_PERIPH_GPIOB;
      break;
    case SSI3_BASE:
      ui32_SysCtl_Periph_SSI = SYSCTL_PERIPH_SSI3;
      ui32_SysCtl_Periph_GPIO = SYSCTL_PERIPH_GPIOD;
      break;
    case SSI0_BASE:
    default:
      ui32_SysCtl_Periph_SSI = SYSCTL_PERIPH_SSI0;
      ui32_SysCtl_Periph_GPIO = SYSCTL_PERIPH_GPIOA;
      break;
  };
  
#ifdef UART_STDIO
  UARTprintf("Configuring MASTER SSI%d, data 16-bit, (0,0) mode, %d bit rate.\n", 0, ui32_bitrate);
#endif // UART_STDIO  
  
  // Enable Periphericals in SysCtl
  MAP_SysCtlPeripheralEnable(ui32_SysCtl_Periph_SSI);
  MAP_SysCtlPeripheralEnable(ui32_SysCtl_Periph_GPIO);
  
  // Enable alternate GPIO functions
  // Configures pin(s) for use by the SSI peripheral
  switch(ui32_SSIx) {
    case SSI1_BASE:
      MAP_GPIOPinConfigure(GPIO_PF2_SSI1CLK);
      MAP_GPIOPinConfigure(GPIO_PF3_SSI1FSS);
      MAP_GPIOPinConfigure(GPIO_PF0_SSI1RX);
      MAP_GPIOPinConfigure(GPIO_PF1_SSI1TX);
      MAP_GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 |
                 GPIO_PIN_0);
      break;
    case SSI2_BASE:
      MAP_GPIOPinConfigure(GPIO_PB4_SSI2CLK);
      MAP_GPIOPinConfigure(GPIO_PB5_SSI2FSS);
      MAP_GPIOPinConfigure(GPIO_PB6_SSI2RX);
      MAP_GPIOPinConfigure(GPIO_PB7_SSI2TX);
      MAP_GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 |
                 GPIO_PIN_7);
      break;
    case SSI3_BASE:
      MAP_GPIOPinConfigure(GPIO_PD0_SSI3CLK);
      MAP_GPIOPinConfigure(GPIO_PD1_SSI3FSS);
      MAP_GPIOPinConfigure(GPIO_PD2_SSI3RX);
      MAP_GPIOPinConfigure(GPIO_PD3_SSI3TX);
      MAP_GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 |
                 GPIO_PIN_3);
      break;
    case SSI0_BASE:
    default:
      MAP_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
      MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
      MAP_GPIOPinConfigure(GPIO_PA4_SSI0RX);
      MAP_GPIOPinConfigure(GPIO_PA5_SSI0TX);
      MAP_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 |
                 GPIO_PIN_2);
      break;
  };  
  
  // Configures the SSI operation mode
  MAP_SSIConfigSetExpClk(ui32_SSIx, MAP_SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,
                       SSI_MODE_MASTER, ui32_bitrate, 16);
  
  // Enables SSI
  MAP_SSIEnable(ui32_SSIx);
  
  while(MAP_SSIDataGetNonBlocking(ui32_SSIx, &ui32_RxData));                       
}

inline uint16_t enc28_SPISend(uint16_t ui16_rw) {
  uint32_t ui32_rx_val;
  MAP_SSIDataPut(ui32_SSIx, ui16_rw);
  while(MAP_SSIBusy(SSI0_BASE));
  MAP_SSIDataGet(ui32_SSIx, &ui32_rx_val);
  return (uint16_t)ui32_rx_val;
}
