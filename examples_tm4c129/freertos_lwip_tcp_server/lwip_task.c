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

#include "freertos_lwip.h"
#include "idle_task.h"
#include "lwip_task.h"

//*****************************************************************************
//
// TCP echo example
//
//*****************************************************************************

static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
static err_t echo_accept(void *arg, struct tcp_pcb *pcb, err_t err);
static void echo_err(void *arg, err_t err);

static err_t echo_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
  UARTprintf("Accepted\n");
  
  struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen*)arg;
  
  // Inform lwIP that an incoming connection has been accepted.
  tcp_accepted(lpcb);
  // Set priority 
  tcp_setprio(pcb, TCP_PRIO_MIN);
  
  // TODO: set argument to be passed
  
  // Set up the various callback functions
  tcp_recv(pcb, echo_recv);
  tcp_err(pcb, echo_err);
  //tcp_poll(pcb, http_poll, HTTPD_POLL_INTERVAL);
  //tcp_sent(pcb, echo_sent);
  
  return ERR_OK;
}

/**
 * Data has been received on this pcb.
 * For HTTP 1.0, this should normally only happen once (if the request fits in one packet).
 */
static err_t echo_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
  err_t parsed = ERR_ABRT;

  //LWIP_DEBUGF(HTTPD_DEBUG | LWIP_DBG_TRACE, ("http_recv: pcb=%p pbuf=%p err=%s\n", (void*)pcb,(void*)p, lwip_strerr(err)));

  if ((err != ERR_OK) || (p == NULL)) {
    /* error or closed by other side? */
    if (p != NULL) {
      /* Inform TCP that we have taken the data. */
      tcp_recved(pcb, p->tot_len);
      pbuf_free(p);
    }
    
    // Close connection
    tcp_arg(pcb, NULL);
    tcp_recv(pcb, NULL);
    tcp_err(pcb, NULL);
    tcp_poll(pcb, NULL, 0);
    tcp_sent(pcb, NULL);
    err = tcp_close(pcb);
    
    return ERR_OK;
  }

  // print data
  if (p->len == p->tot_len) UARTprintf("1 pbuf\n");
  UARTprintf("len: %d\n", p->len);
  
  char data_rcv[256];
  uint32_t data_len = p->len;
  if (p->len < 256)
  {
    data_rcv[p->len] = '\0';
    pbuf_copy_partial(p, data_rcv, p->len, 0);
  }
  UARTprintf("%s\n", data_rcv);  
  
  // Inform TCP that we have taken the data.
  tcp_recved(pcb, p->tot_len);

  
  tcp_write(pcb, data_rcv, data_len, TCP_WRITE_FLAG_COPY);

  pbuf_free(p);
  tcp_close(pcb);

  return ERR_OK;
}

/**
 * The pcb had an error and is already deallocated.
 * The argument might still be valid (if != NULL).
 */
static void echo_err(void *arg, err_t err)
{
  UARTprintf("http_err: %s", lwip_strerr(err));
}
  

//*****************************************************************************
//
// Sets up the additional lwIP raw API services provided by the application.
//
//*****************************************************************************
void
SetupServices(void *pvArg)
{
    //
    // Initialize the sample tcp echo
    //
    struct tcp_pcb *pcb;
    err_t err;

    pcb = tcp_new();
    
    tcp_setprio(pcb, TCP_PRIO_MIN);

    err = tcp_bind(pcb, IP_ADDR_ANY, 8080);
    
    //returns a new connection identifier, and the one passed as an argument to the function will be deallocated.    
    pcb = tcp_listen(pcb);
    
    /* initialize callback arg and accept callback */
    tcp_arg(pcb, pcb);
    tcp_accept(pcb, echo_accept);
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
    // Setup the remaining services inside the TCP/IP thread's context.
    //
    tcpip_callback(SetupServices, 0);

    //
    // Success.
    //
    return(0);
}
