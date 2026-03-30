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

#include "canpanel.h"
#include "FLiM.h"
#include "panelFLiM.h"
#include "panelEvents.h"
#include "callbacks.h"
#include "max6951.h"
#include "hardCoded.h"
#include "canpanel.h"

// Local prototypes


#pragma code APP
#pragma romdata

// Array of buttons and their LEDs and feedbacks 

#define RUN_LED1 26  // LED number to light when up and running
#define RUN_LED2 30  // LED number to light when up and running
#define RUN_LED3 27  // LED number to light when up and running
#define RUN_LED4 31  // LED number to light when up and running





const rom BYTE    hardCodedButtons[HARDCODED_MAX_BUTTON] = {86,87,65,00,00,00,00,00,00,00,00,83,67,66,98,70,00,00,00,00,00,80,96,112,49,00,70,00,00,55,118,48,32,33,00,00,00,00,00,103,00,97,00,21,
                                                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39,7,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,64,81,37};
const rom BYTE    hardCodedLeds[HARDCODED_MAX_BUTTON] =    {48,56,64 ,33,41,49,42,50,35,43,51,44,45,53 ,36,37};
const rom BYTE    ledGroupLimits[LED_GROUPS] = {1,1};

#pragma udata MAIN_VARS


BYTE    ledNum;
BYTE    segCount;
BYTE    waitingRoute;

TickValue  routeRequestedTime;


   

// eventsInit called during initialisation - initialises event support 

void initHardCoded(void)

{
    ledNum = 1;
    segCount = 0;
    waitingRoute = 0;       // No route waiting for setup
    
#ifdef KSIGNALS
#elif defined KSIGNALMAN
#else    
    setLed(RUN_LED1, TRUE );
    setLed(RUN_LED2, TRUE );
    setLed(RUN_LED3, TRUE );
    setLed(RUN_LED4, TRUE );
#endif
    
} // eventsInit


BYTE hardCodedProducerMap( BYTE button )

{
    BYTE i;
    BYTE mappedButton;
            
    
#ifdef BURNDEN   
    mappedButton = 99;  // Send event 99 if mapping fails
#else
    mappedButton = button;  // Send unchanged button number if mapping fails
            
    for (i=0; i<HARDCODED_MAX_BUTTON; i++)
        if (hardCodedButtons[i] == button) 
            mappedButton = i+1;
    
    // Special cases
    
#ifdef BURNDEN
    
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
    
#endif
    
    return(mappedButton);
}

void clearhardCodedLeds( BYTE groupId )

{
    BYTE i;
  
    for (i=0;i<HARDCODED_MAX_LED;i++)
    {    
        if ((groupId != 0xFF) && (HardCodedEvents[i].groupId == groupId))
           setLed(HardCodedEvents[i].ledNumber, FALSE);
    }    
    
//  clearAllLeds();
    
//    if (groupId < LED_GROUPS)
//    {
//         
//        for (i=ledGroupLimits[groupId]; i<ledGroupLimits[groupId+1]; i++)
//                setLed(hardCodedLeds[i], FALSE);
//    }
}

void TurnOnNextLed()

{
    clearAllLeds();
     setLed(ledNum++, TRUE);
}    

// Flashes selected LED instead of acting on incoming event

void hardCodedFlashSelected( BYTE button )
{
    int aIndex;
     
       

  
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

BYTE findHardCodedEvent( BYTE startIndex, WORD eventNode, WORD eventNum  )

{
    BYTE    i;
    BYTE    eventIndex;
    
    eventIndex = 0xFF;
    
    for (i=startIndex;i<HARDCODED_MAX_LED*HARDCODED_LED_STATES;i++)
        if ((HardCodedEvents[i].evNodeNum == eventNode) && (HardCodedEvents[i].evEventNum == eventNum))
        {    
            eventIndex = i;
            break;
        }    
    
    return( eventIndex);
}

BOOL processHardCodedEvent( SoDStatusP sodStat, WORD eventNode, WORD eventNum, BYTE eventIndex, BYTE *msg )

{
    BOOL    eventProcessed = TRUE;
    BOOL    onEvent;
    
    // Process short event ON with node number zero
    // or long event on with node number non-zero
    
    onEvent = ((eventNode == 0) && (msg[d0] == OPC_ASON)) 
            ||((eventNode != 0) && (msg[d0] == OPC_ACON));
    
    switch (HardCodedEvents[eventIndex].ledAction)
    {
        case evActFlashLed:
            if (onEvent)
            {
                clearhardCodedLeds(HardCodedEvents[eventIndex].groupId);
                flashLed(HardCodedEvents[eventIndex].ledNumber);
            }    
            break;
            
        case evActLedOn:
            clearhardCodedLeds(HardCodedEvents[eventIndex].groupId);
            // drop through
        case evActLedOff:
            if (onEvent)
                setLed(HardCodedEvents[eventIndex].ledNumber, HardCodedEvents[eventIndex].ledAction );
            break;
        
        case evActLedFollow:
            setLed(HardCodedEvents[eventIndex].ledNumber, onEvent );
            break;
            
        case evActLedFollowInv:    
            setLed(HardCodedEvents[eventIndex].ledNumber, !onEvent );
            break;
            
        case evNextLed:  // Used to diagnose which LED lights for each LED number - sends event with the event number equal to the LED number it is lighting, steps through LEDs sequentially each time called
            clearAllLeds(); 
            cbusSendEvent( 0, -1, ledNum, TRUE );
            setLed( ledNum++, TRUE );
            break;
            
        case evSod:   // Send start of day status for each button/switch unless excluded from SoD
            doButtonsSod(sodStat);
            break;
            
        default:
            eventProcessed = FALSE;
    }
   
    if (HardCodedEvents[eventIndex].buttonFollow != bfNone)
    {
        setButtonState( HardCodedEvents[eventIndex].followButton, (onEvent ^ (HardCodedEvents[eventIndex].buttonFollow == bfFollowInv) ));
    }    
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






