#ifndef _UART_PROTOCOL_H_
#define _UART_PROTOCOL_H_

#include "utils/ringbuf.h"

#define RX_BUFFER_SIZE 128


extern tRingBufObject g_tBuffRx;
extern uint8_t g_ui8RxData[RX_BUFFER_SIZE];

void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void initUART();
void UARTParse();

void UARTIntHandler(void);

#endif // _UART_PROTOCOL_H_
