#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/sw_crc.h"

#include "utils/ringbuf.h"
#include "utils/uartstdio.h"

#include "uart_protocol.h"

tRingBufObject g_tBuffRx;
uint8_t g_ui8RxData[RX_BUFFER_SIZE];

//*****************************************************************************
// Check for a packet on the Ringbuffer
//*****************************************************************************
void UARTParse()
{
  uint8_t ui8_dataLen;
  uint16_t ui16_crc_result;
  
  // Could it have at least one frame?
  while(RingBufUsed(&g_tBuffRx) > 4) 
  {
    // Does it start with frame start flag?
    if(g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex] == 0x7E 
      && g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex + 1] == 0x7E )
    {
      ui8_dataLen = g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex + 3];
      // Does it have the whole frame? i.e. data payload + 2x start + ctrl + len + crc16
      if( ui8_dataLen + 6 <= RingBufUsed(&g_tBuffRx) )
      {
        // compute CRC16 of everything except 
        ui16_crc_result = Crc16(0x00, &g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex], ui8_dataLen + 4);
        // If CRC match
        if(ui16_crc_result == ( (g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex+4+ui8_dataLen]<<8) + g_tBuffRx.pui8Buf[g_tBuffRx.ui32ReadIndex+5+ui8_dataLen]) ) 
        {
          //****************************************************************************
          //
          // Put here what to do with your serial commands
          //
          //****************************************************************************
          
          UARTprintf("\nCRC check ok! %d\n", ui16_crc_result);

          // We have a command to execute
          UARTprintf("\nGot packet: ");
          for(int i = 0; i < ui8_dataLen + 6; i++)
          {
            UARTCharPut(UART0_BASE, RingBufReadOne(&g_tBuffRx));
          }
          
          
          //****************************************************************************
        }
      } else 
      {
        // Wait for more data
        break;
      }
    } else // Next try in finding a frame
    {
      RingBufAdvanceRead(&g_tBuffRx, 1);
    }
  }
}

//*****************************************************************************
// The UART interrupt handler.
//*****************************************************************************
void UARTIntHandler(void)
{
    uint32_t ui32Status;

    // Get the interrrupt status.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    // Clear the asserted interrupts.
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        // Read the next character from the UART and write it back to the UART.
        //ROM_UARTCharPutNonBlocking(UART0_BASE,
        //                           ROM_UARTCharGetNonBlocking(UART0_BASE));
        RingBufWriteOne( &g_tBuffRx, ROM_UARTCharGetNonBlocking(UART0_BASE) );
    }
}

//*****************************************************************************
// Send a string to the UART.
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    // Loop while there are more characters to send.
    while(ui32Count--)
    {
        // Write the next character to the UART.
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}

//*****************************************************************************
// Init communication peripherals
//*****************************************************************************
void initUART()
{
  // Enable UART0
  ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  
  // Set GPIO A0 and A1 as UART pins.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
  
  // Configure the UART for 115,200, 8-N-1 operation.
  ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
                          (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                           UART_CONFIG_PAR_NONE));

  // Enable the UART interrupt.
  ROM_IntEnable(INT_UART0);
  // Receive time out and receive interrupt
  ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
  
  // Set up the ring buffer
  RingBufInit(&g_tBuffRx, g_ui8RxData, RX_BUFFER_SIZE);
  
  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 115200, 16000000);
}
