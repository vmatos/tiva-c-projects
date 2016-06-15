//*****************************************************************************
//
// lwip_task.c - Tasks to serve web pages over Ethernet using lwIP.
//
// Copyright (c) 2009-2015 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.1.71 of the DK-TM4C129X Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/rom.h"
#include "utils/lwiplib.h"
#include "utils/locator.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "task.h"

#include "freertos_lwip.h"
#include "idle_task.h"
#include "lwip_task.h"

//*****************************************************************************
//
// TCP echo example
//
//*****************************************************************************

static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
  

//*****************************************************************************
//
// Sets up the additional lwIP raw API services provided by the application.
//
//*****************************************************************************
err_t TCPConnected(void *pvArg, struct tcp_pcb *psPcb, err_t iErr);
static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  // print data  
  char data_rcv[256];
  uint32_t data_len = p->len;
  if (p->len < 256)
  {
    data_rcv[p->len] = '\0';
    pbuf_copy_partial(p, data_rcv, p->len, 0);
  }
  UARTprintf("%s\n", data_rcv);  
  
  return ERR_OK;
}

void TCPConnect(void *pvArg)
{
    while (lwIPLocalIPAddrGet() == 0xffffffff || lwIPLocalIPAddrGet() == 0)
    {
      const uint32_t xDelay = 500 / configTICK_RATE_HZ;
      vTaskDelay( xDelay );
    }

    //
    // Initialize the sample tcp echo
    //
    struct tcp_pcb *pcb;
    err_t err;
    pcb = tcp_new();
    
    tcp_setprio(pcb, TCP_PRIO_MIN);

    struct ip_addr server_ip;
    IP4_ADDR(&server_ip, 192,168,1,135);
    
    tcp_recv(pcb, echo_recv);
    

    UARTprintf("Connecting to: 192.168.1.135\n");
    // Only sets up connection. Doesnt return success or not of connection.
    // Success or not are stablished by the callback specified on tcp_err 
    tcp_connect(pcb, &server_ip, 8080, TCPConnected);
    
    uint32_t xDelay = 2000 / configTICK_RATE_HZ;
    vTaskDelay( xDelay );
    
    UARTprintf("Done\n");
    
    // Close and delete callbacks
    tcp_recv(pcb, NULL);
    tcp_close(pcb);
    
    while (1)
    {
      uint32_t xDelay = 2000 / configTICK_RATE_HZ;
      vTaskDelay( xDelay );
    }
}

err_t TCPConnected(void *pvArg, struct tcp_pcb *psPcb, err_t iErr)
{
    // Check if there was a TCP error.
    if(iErr != ERR_OK)
    {
        // Clear out all of the TCP callbacks.
        tcp_sent(psPcb, NULL);
        tcp_recv(psPcb, NULL);
        tcp_err(psPcb, NULL);
        // Close the TCP connection.
        tcp_close(psPcb);
        // And return.
        return(ERR_CONN);
    }
    UARTprintf("Connected\n");

    // Setup the TCP receive function.
    //~ tcp_recv(psPcb, TCPReceived);

    // Setup the TCP error function.
    //~ tcp_err(psPcb, TCPError);

    // Setup the TCP sent callback function.
    //~ tcp_sent(psPcb, TCPSent);

    char data_send[] = "Hello!\n";
    tcp_write(psPcb, data_send, 7, TCP_WRITE_FLAG_COPY);

    // Return a success code.
    return(ERR_OK);
}

//*****************************************************************************
//
// Initializes the lwIP tasks.
//
//*****************************************************************************
uint32_t
lwIPTaskInit(void)
{
    uint32_t ui32User0, ui32User1;
    uint8_t pui8MAC[6];

    //
    // Get the MAC address from the user registers.
    //
    ROM_FlashUserGet(&ui32User0, &ui32User1);
    if((ui32User0 == 0xffffffff) || (ui32User1 == 0xffffffff))
    {
        return(1);
    }

    //
    // Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
    // address needed to program the hardware registers, then program the MAC
    // address into the Ethernet Controller registers.
    //
    pui8MAC[0] = ((ui32User0 >>  0) & 0xff);
    pui8MAC[1] = ((ui32User0 >>  8) & 0xff);
    pui8MAC[2] = ((ui32User0 >> 16) & 0xff);
    pui8MAC[3] = ((ui32User1 >>  0) & 0xff);
    pui8MAC[4] = ((ui32User1 >>  8) & 0xff);
    pui8MAC[5] = ((ui32User1 >> 16) & 0xff);

    //
    // Lower the priority of the Ethernet interrupt handler.  This is required
    // so that the interrupt handler can safely call the interrupt-safe
    // FreeRTOS functions (specifically to send messages to the queue).
    //
    ROM_IntPrioritySet(INT_EMAC0, 0xC0);

    //
    // Initialize lwIP.
    //
    // Creates FreeRTOS task throught utils/lwiplib.c
    lwIPInit(g_ui32SysClock, pui8MAC, 0, 0, 0, IPADDR_USE_DHCP);

    //
    // Connect to server
    //
    xTaskCreate(TCPConnect, (const portCHAR *)"TCPconnect", 256, 0, tskIDLE_PRIORITY + 1, 0);

    //
    // Success.
    //
    return(0);
}
