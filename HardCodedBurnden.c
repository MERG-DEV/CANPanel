/*

 Copyright (C) Pete Brownlow 2018   software@upsys.co.uk

 Routines for CBUS event management - hard coded version for Burnden Park project

  This work is licensed under the:
      Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
   To view a copy of this license, visit:
      http://creativecommons.org/licenses/by-nc-sa/4.0/
   or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

   License summary:
    You are free to:
      Share, copy and redistribute the material in any medium or format
      Adapt, remix, transform, and build upon the material

    The licensor cannot revoke these freedoms as long as you follow the license terms.

    Attribution : You must give appropriate credit, provide a link to the license,
                   and indicate if changes were made. You may do so in any reasonable manner,
                   but not in any way that suggests the licensor endorses you or your use.

    NonCommercial : You may not use the material for commercial purposes. **(see note below)

    ShareAlike : If you remix, transform, or build upon the material, you must distribute
                  your contributions under the same license as the original.

    No additional restrictions : You may not apply legal terms or technological measures that
                                  legally restrict others from doing anything the license permits.

   ** For commercial use, please contact the original copyright holder(s) to agree licensing terms

    This software is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE

**************************************************************************************************************
  Note:   This source code has been written using a tab stop and indentation setting
          of 4 characters. To see everything lined up correctly, please set your
          IDE or text editor to the same settings.
******************************************************************************************************
	
 For library version number and revision history see CBUSLib.h


	These event routines have no code or definitions that are specific to any
	module, so they can be used to provide event facilities for any module 
	using these libraries.
	
 History for this file:
	13/11/8   	Pete Brownlow	- Cloned from events.c
 

 */

#include "FLiM.h"
#include "callbacks.h"
#include "max6951.h"
#include "hardCoded.h"

// Local prototypes


#pragma code APP
#pragma romdata

// Array of buttons and their LEDs and feedbacks 

#define RUN_LED 23  // LED number to light when up and running

const rom BYTE    hardCodedButtons[HARDCODED_MAX_BUTTON] = {80,96,112,65,81,97,82,98,67,83,99,84,69,101,70,86,118};
const rom BYTE    hardCodedLeds[HARDCODED_MAX_BUTTON] =    {48,56,64 ,33,41,49,42,50,35,43,51,44,45,53 ,36,37,62};
const rom BYTE    ledGroupLimits[LED_GROUPS+1] = {0,MAX_STORAGE_ROAD, HARDCODED_MAX_BUTTON};

const rom HCEvTable HardCodedEvents[] = 
{
//   Button events that start LED flashing
    {0,1,48,0,evActFlashLed},    // Node Number, event number, LED number, action
    {0,2,56,0,evActFlashLed},
    {0,3,64,0,evActFlashLed},
    {0,4,33,0,evActFlashLed},   
    {0,5,41,0,evActFlashLed},
    {0,6,49,0,evActFlashLed},
    {0,7,42,0,evActFlashLed},
    {0,8,50,0,evActFlashLed},
    {0,9,35,0,evActFlashLed},
    {0,10,43,0,evActFlashLed},
    {0,11,51,0,evActFlashLed},
    {0,12,44,0,evActFlashLed},
    {0,13,45,0,evActFlashLed},
    {0,20,53,1,evActFlashLed},
    {0,21,36,1,evActFlashLed},
    {0,22,37,1,evActFlashLed},
    {0,23,62,1,evActFlashLed},
//    Feedback events from CANCOMPUTE(s) that set LED on steady
    {80,1,48,0,evActLedOn},    // Node Number, event number, LED number, action
    {80,2,56,0,evActLedOn},
    {80,3,64,0,evActLedOn},
    {80,4,33,0,evActLedOn},   
    {80,5,41,0,evActLedOn},
    {80,6,49,0,evActLedOn},
    {80,7,42,0,evActLedOn},
    {80,8,50,0,evActLedOn},
    {80,9,35,0,evActLedOn},
    {80,10,43,0,evActLedOn},
    {80,11,51,0,evActLedOn},
    {80,12,44,0,evActLedOn},
    {80,13,45,0,evActLedOn},
    {80,14,53,1,evActLedOn},
    {80,15,36,1,evActLedOn},
    {80,16,37,1,evActLedOn},
    {80,17,62,1,evActLedOn},
    
};      
        

#pragma udata MAIN_VARS


BYTE    ledNum;
BYTE    waitingRoute;

TickValue  routeRequestedTime;


   

// eventsInit called during initialisation - initialises event support 

void initHardCoded(void)

{
//    ledNum = 35;
    waitingRoute = 0;       // No route waiting for setup
    setLed(RUN_LED, TRUE );
} // eventsInit


BYTE hardCodedProducerMap( BYTE button )

{
    BYTE i;
    BYTE mappedButton;
            
    
    mappedButton = 99;  // Send event 99 if mapping fails
            
    for (i=0; i<HARDCODED_MAX_BUTTON; i++)
        if (hardCodedButtons[i] == button) 
            mappedButton = i+1;
    
    // Special cases
    
    if (mappedButton == 13)
        mappedButton = 21;  // IN
    
    if (mappedButton == 14)
        mappedButton = 20;  // OUT
    
    if (mappedButton == 15)
        mappedButton = 22;  // SHUNT
    
    if (mappedButton == 16)
        mappedButton = 13;  // RELEASE (Road 13))
      
    if (mappedButton == 17)
        mappedButton = 23;  // NEXT
    
    return(mappedButton);
}

void clearhardCodedLeds( BYTE groupId )

{
    BYTE i;
    
    if (groupId < LED_GROUPS)
    {
        for (i=ledGroupLimits[groupId]; i<ledGroupLimits[groupId+1]; i++)
                setLed(hardCodedLeds[i], FALSE);
    }
}


// Flashes selected LED instead of acting on incoming event

void hardCodedFlashSelected( BYTE button )
{
    int aIndex;
     
       
//    digcount = ledNum / 8
//       segCount = (segCount = 0 ? 1 : segCount<<1);    // Next segment bit in digit byte
//   
//    sendMxCmd( MX_DIG_BOTH + digCount, segCount);   // Turn on one segment
// 
  
    BYTE i;
    
    if (button <= MAX_STORAGE_ROAD)
    {    
        for (i=0; i<MAX_STORAGE_ROAD; i++)
            setLed(hardCodedLeds[i],FALSE);
    
        flashLed(hardCodedLeds[button-1]);
    }
    else
    {
        for (i=MAX_STORAGE_ROAD; i<HARDCODED_MAX_BUTTON; i++)
            setLed(hardCodedLeds[i],FALSE);
        
        // Special cases

        if (button == 21)
            flashLed(36);  // IN

        if (button == 20)
            flashLed(53);  // OUT

        if (button == 22)
            flashLed(37);  // SHUNT

        if (button == 23)
            flashLed(62);  // NEXT
    }
    waitingRoute = button;
    routeRequestedTime.Val = tickGet();    
}

void checkWaitingRoutes()

{
    if (waitingRoute != 0)
    {
        if (tickTimeSince(routeRequestedTime) > ROUTE_SETUP_TIME)
        {    
            if (waitingRoute <= MAX_STORAGE_ROAD)
                setLed(hardCodedLeds[waitingRoute-1], TRUE);
            else
            {
                if (waitingRoute == 21)
                    setLed(36, TRUE);  // IN

                if (waitingRoute == 20)
                    setLed(53, TRUE);  // OUT

                if (waitingRoute == 22)
                    setLed(37, TRUE);  // SHUNT

                if (waitingRoute == 23)
                    setLed(62, TRUE);  // NEXT
            }  
            
            waitingRoute = 0;
        }        
    }    
}

// Returns index into hard coded event table

BYTE findHardCodedEvent( WORD eventNode, WORD eventNum  )

{
    BYTE    i;
    BYTE    eventIndex;
    
    eventIndex = 0xFF;
    
    for (i=0;i<HARDCODED_MAX_BUTTON*2;i++)
        if ((HardCodedEvents[i].evNodeNum == eventNode) && (HardCodedEvents[i].evEventNum == eventNum))
            eventIndex = i;
    
    return( eventIndex);
}

BOOL processHardCodedEvent( WORD eventNode, WORD eventNum, BYTE eventIndex, BYTE *msg )

{
    BOOL    eventProcessed = TRUE;
    
    // Hard Coded only processes ON events
    // Process short event ON with node number zero
    // or long event on with node number non-zero
    
    if (((eventNode == 0) && (msg[d0] == OPC_ASON)) 
    ||  ((eventNode != 0) && (msg[d0] == OPC_ACON))) 
    {
        clearhardCodedLeds(HardCodedEvents[eventIndex].groupId);
        if (HardCodedEvents[eventIndex].ledAction == evActFlashLed)
            flashLed(HardCodedEvents[eventIndex].ledNumber);
        else
            setLed(HardCodedEvents[eventIndex].ledNumber, HardCodedEvents[eventIndex].ledAction );
    }
    else
        eventProcessed = FALSE;
    
    return( eventProcessed );
}
 
        
//    switch(msg[d0])
//    {
//        case OPC_ACON:     //
//            if (thisNN(msg))
//                break;  // Ignore if for us (already in learn) but drop through to exit learn mode if not addressed to us
//
//        case OPC_NNULN:
//            // Release node from learn mode
//             flimState = fsFLiM;
//            break;
//    }





