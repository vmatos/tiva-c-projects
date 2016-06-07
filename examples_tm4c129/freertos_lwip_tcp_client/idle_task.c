//*****************************************************************************
//
// idle_task.c - The FreeRTOS idle task.
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
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "utils/lwiplib.h"
#include "utils/uartstdio.h"
#include "lwip/stats.h"
#include "idle_task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
//*****************************************************************************
//
// The stack for the idle task.
//
//*****************************************************************************
uint32_t g_pui32IdleTaskStack[128];


//*****************************************************************************
//
// The number of seconds that the application has been running.  This is
// initialized to -1 in order to get the initial display updated as soon as
// possible.
//
//*****************************************************************************
static uint32_t g_ui32Seconds = 0xffffffff;

//*****************************************************************************
//
// The current IP address.  This is initialized to -1 in order to get the
// initial display updated as soon as possible.
//
//*****************************************************************************
static uint32_t g_ui32IPAddress = 0xffffffff;

//*****************************************************************************
//
// The number of packets that have been transmitted.  This is initialized to -1
// in order to get the initial display updated as soon as possible.
//
//*****************************************************************************
static uint32_t g_ui32TXPackets = 0xffffffff;

//*****************************************************************************
//
// The number of packets that have been received.  This is initialized to -1 in
// order to get the initial display updated as soon as possible.
//
//*****************************************************************************
static uint32_t g_ui32RXPackets = 0xffffffff;

//*****************************************************************************
//
// A buffer to contain the string versions of the information displayed at the
// bottom of the display.
//
//*****************************************************************************
static char g_pcTimeString[12];
static char g_pcIPString[24];


//*****************************************************************************
//
// Displays the IP address in a human-readable format.
//
//*****************************************************************************
static void
DisplayIP(uint32_t ui32IP)
{
    uint32_t ui32Loop, ui32Idx, ui32Value;

    //
    // If there is no IP address, indicate that one is being acquired.
    //
    if(ui32IP == 0)
    {
        UARTprintf("  Acquiring IP...  \n");
        return;
    }

    //
    // Set the initial index into the string that is being constructed.
    //
    ui32Idx = 0;

    //
    // Start the string with four spaces.  Not all will necessarily be used,
    // depending upon the length of the IP address string.
    //
    for(ui32Loop = 0; ui32Loop < 4; ui32Loop++)
    {
        g_pcIPString[ui32Idx++] = ' ';
    }

    //
    // Loop through the four bytes of the IP address.
    //
    for(ui32Loop = 0; ui32Loop < 32; ui32Loop += 8)
    {
        //
        // Extract this byte from the IP address word.
        //
        ui32Value = (ui32IP >> ui32Loop) & 0xff;

        //
        // Convert this byte into ASCII, using only the characters required.
        //
        if(ui32Value > 99)
        {
            g_pcIPString[ui32Idx++] = '0' + (ui32Value / 100);
        }
        if(ui32Value > 9)
        {
            g_pcIPString[ui32Idx++] = '0' + ((ui32Value / 10) % 10);
        }
        g_pcIPString[ui32Idx++] = '0' + (ui32Value % 10);

        //
        // Add a dot to separate this byte from the next.
        //
        g_pcIPString[ui32Idx++] = '.';
    }

    //
    // Fill the remainder of the string buffer with spaces.
    //
    for(ui32Loop = ui32Idx - 1; ui32Loop < 20; ui32Loop++)
    {
        g_pcIPString[ui32Loop] = ' ';
    }

    //
    // Null terminate the string at the appropriate place, based on the length
    // of the string version of the IP address.  There may or may not be
    // trailing spaces that remain.
    //
    g_pcIPString[ui32Idx + 3 - ((ui32Idx - 12) / 2)] = '\0';

    //
    // Display the string.  The horizontal position and the number of leading
    // spaces utilized depend on the length of the string version of the IP
    // address.  The end result is the IP address centered in the provided
    // space with leading/trailing spaces as required to clear the remainder of
    // the space.
    //
    UARTprintf("%s\n", g_pcIPString + ((ui32Idx - 12) / 2));
}

//*****************************************************************************
//
// This hook is called by the FreeRTOS idle task when no other tasks are
// runnable.
//
//*****************************************************************************
void
vApplicationIdleHook(void)
{
    uint32_t ui32Temp;

    //
    // Get the number of seconds that the application has been running.
    //
    //~ ui32Temp = xTaskGetTickCount() / (1000 / portTICK_RATE_MS);

    //
    // See if the number of seconds has changed.
    //
    //~ if(ui32Temp != g_ui32Seconds)
    //~ {
        //~ //
        //~ // Update the local copy of the run time.
        //~ //
        //~ g_ui32Seconds = ui32Temp;
//~ 
        //~ //
        //~ // Convert the number of seconds into a text string.
        //~ //
        //~ g_pcTimeString[0] = '0' + ((ui32Temp / 36000) % 10);
        //~ g_pcTimeString[1] = '0' + ((ui32Temp / 3600) % 10);
        //~ g_pcTimeString[2] = ':';
        //~ g_pcTimeString[3] = '0' + ((ui32Temp / 600) % 6);
        //~ g_pcTimeString[4] = '0' + ((ui32Temp / 60) % 10);
        //~ g_pcTimeString[5] = ':';
        //~ g_pcTimeString[6] = '0' + ((ui32Temp / 10) % 6);
        //~ g_pcTimeString[7] = '0' + (ui32Temp % 10);
        //~ g_pcTimeString[8] = '\0';
//~ 
        //~ //
        //~ // Have the display task write this string onto the display.
        //~ //
        //~ UARTprintf("%s\n", g_pcTimeString);
    //~ }

    //
    // Get the current IP address.
    //
    ui32Temp = lwIPLocalIPAddrGet();

    //
    // See if the IP address has changed.
    //
    if(ui32Temp != g_ui32IPAddress)
    {
        //
        // Save the current IP address.
        //
        g_ui32IPAddress = ui32Temp;

        //
        // Update the display of the IP address.
        //
        DisplayIP(ui32Temp);
    }

    //
    // See if the number of transmitted packets has changed.
    //
    //~ if(lwip_stats.link.xmit != g_ui32TXPackets)
    //~ {
        //~ //
        //~ // Save the number of transmitted packets.
        //~ //
        //~ ui32Temp = g_ui32TXPackets = lwip_stats.link.xmit;
//~ 
        //~ //
        //~ // Update the display of transmitted packets.
        //~ //
        //~ UARTprintf("Tx: %d\n", ui32Temp);
    //~ }

    //
    // See if the number of received packets has changed.
    //
    //~ if(lwip_stats.link.recv != g_ui32RXPackets)
    //~ {
        //~ //
        //~ // Save the number of received packets.
        //~ //
        //~ ui32Temp = g_ui32RXPackets = lwip_stats.link.recv;
//~ 
        //~ //
        //~ // Update the display of received packets.
        //~ //
        //~ UARTprintf("Rx: %d\n", ui32Temp);
    //~ }

}
