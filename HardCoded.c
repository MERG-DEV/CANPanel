/*

 Copyright (C) Pete Brownlow 2018   software@upsys.co.uk

 Routines for CBUS event management - hard coded version 

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

#define RUN_LED1 26  // LED number to light when up and running
#define RUN_LED2 30  // LED number to light when up and running
#define RUN_LED3 27  // LED number to light when up and running
#define RUN_LED4 31  // LED number to light when up and running





const rom BYTE    hardCodedButtons[HARDCODED_MAX_BUTTON] = {64,80,96,112,81,97,82,98,84,100,69,101,70,86,65,66};
const rom BYTE    hardCodedLeds[HARDCODED_MAX_BUTTON] =    {48,56,64 ,33,41,49,42,50,35,43,51,44,45,53 ,36,37};
const rom BYTE    ledGroupLimits[LED_GROUPS+1] = {1,10,20,30,40};

const rom HCEvTable HardCodedEvents[] = 
{   
#ifdef KFY
//   Button events that start LED flashing - Node NumbeFr, event number, LED number, Group, action
    {450,64,25,4,evActFlashLed},   // FYUPBH correct
//    {450,64,1,0,evActFlashLed},   // FYUPBH dummy led always out
    {450,80,32,0,evActFlashLed},    // FY01A
    {450,96,24,1,evActFlashLed},     // FY01D
    {450,112,8,1,evActFlashLed},     // FYUPVH   
//    {450,112,1,5,evActFlashLed},     // FYUPVH   
    
    {450,81,16,0,evActFlashLed},     // FY02A
    {450,97,17,1,evActFlashLed},     // FY02D
    {450,82,9,0,evActFlashLed},      // FY03A
    {450,98,18,1,evActFlashLed},     // FY03D
    {451,84,11,2,evActFlashLed},    // FY04D
    {451,100,20,3,evActFlashLed},     // FY04A
    {451,69,12,2,evActFlashLed},    // FY05D
    {451,101,21,3,evActFlashLed},     // FY05A
    {451,70,28,2,evActFlashLed},    // FYREL
//    {451,70,1,6,evActFlashLed},    // FYREL
    
    {451,86,13,2,evActFlashLed},    // FY06D
    {451,65,22,3,evActFlashLed},    // FY06A
    {451,66,23,3,evActFlashLed},    // FYDNVH
//    {451,66,1,7,evActFlashLed},    // FYDNVH
    
  //    Feedback events from CANCOMPUTE(s) that set LED on steady - Node Number, event number, LED number, action
    {80,13,25,4,evActLedOn},    // FYUPBH correct
//    {80,13,1,4,evActLedOn},    // FYUPBH dummy led always out
    {80,7,32,0,evActLedOn},     // FY01A
    {80,1,24,1,evActLedOn},     // FY01D
    {80,14,8,5,evActLedOn},     // FYUPVH
//    {80,14,1,5,evActLedOn},     // FYUPVH
    
    {80,8,16,0,evActLedOn},     // FY02A
    {80,2,17,1,evActLedOn},     // FY02D
    {80,9,9,0,evActLedOn},      // FY03A
    {80,3,18,1,evActLedOn},     // FY03D
    {80,10,11,2,evActLedOn},    // FY04D
    {80,4,20,3,evActLedOn},     // FY04A
    {80,11,12,2,evActLedOn},    // FY05D
    {80,5,21,3,evActLedOn},     // FY05A
    {80,15,28,6,evActLedOn},    // FYREL
//    {80,15,1,6,evActLedOn},    // FYREL
    
    {80,12,13,2,evActLedOn},    // FY06D
    {80,6,22,3,evActLedOn},    // FY06A
    {80,16,23,7,evActLedOn}     // FYDNVH
//    {80,16,1,7,evActLedOn}     // FYDNVH
    
};      

#elif defined KSIGNALS
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action
    {431,9,60,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,61,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,48,0xFF,evActLedFollow},     // FB: Up Starter Clear
    {432,10,41,0xFF,evActLedFollow},    // FB: Up Starter Danger      
    {432,11,64,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,57,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,40,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,33,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,50,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,51,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,12,56,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,13,49,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,14,58,0xFF,evActLedFollow},     // FB: MPD Exit Clear       
    {435,15,59,0xFF,evActLedFollow},     // FB: MPD Exit Danger
    {452,103,9,0xFF,evActLedFollow}     // Emergency stop all
    
};

#elif defined MSM
//   Button events that start LED flashing - Node NumbeFr, event number, LED number, Group, action
    {462,64,25,4,evActFlashLed},   // FYUPBH correct
    {462,64,1,0,evActFlashLed},   // FYUPBH dummy led always out
    {462,80,32,0,evActFlashLed},    // FY01A
    {462,96,24,1,evActFlashLed},     // FY01D
    {462,112,8,1,evActFlashLed},     // FYUPVH   
    {462,112,1,5,evActFlashLed},     // FYUPVH   
    
    {462,81,16,0,evActFlashLed},     // FY02A
    {462,97,17,1,evActFlashLed},     // FY02D
    {462,82,9,0,evActFlashLed},      // FY03A
    {462,98,18,1,evActFlashLed},     // FY03D
    {462,84,11,2,evActFlashLed},    // FY04D
    {462,100,20,3,evActFlashLed},     // FY04A
    {462,69,12,2,evActFlashLed},    // FY05D
    {462,101,21,3,evActFlashLed},     // FY05A
    {462,70,28,2,evActFlashLed},    // FYREL
    {462,70,1,6,evActFlashLed},    // FYREL
    
    {462,86,13,2,evActFlashLed},    // FY06D
    {462,65,22,3,evActFlashLed},    // FY06A
    {462,66,23,3,evActFlashLed},    // FYDNVH
    {462,66,1,7,evActFlashLed},    // FYDNVH
    {462,66,23,3,evActFlashLed},    // FYDNVH
    {462,66,1,7,evActFlashLed},    // FYDNVH            
    
  //    Feedback events from CANCOMPUTE(s) that set LED on steady - Node Number, event number, LED number, group, action
    {80,1,24,1,evActLedOn},     // FY01D
    {80,2,17,1,evActLedOn},     // FY02D
    {80,3,18,1,evActLedOn},     // FY03D
    {80,4,20,1,evActLedOn},     // FY04D
            
    {80,11,24,1,evActLedOn},     // PL01D
    {80,12,17,1,evActLedOn},     // PL02D
    {80,13,18,1,evActLedOn},     // PL03D
    {80,14,20,1,evActLedOn},     // HS01DM
    {80,15,20,1,evActLedOn},     // HS01DG            
 
    {80,21,24,1,evActLedOn},     // FY01A
    {80,22,17,1,evActLedOn},     // FY02A
    {80,23,18,1,evActLedOn},     // FY03A
    {80,24,20,1,evActLedOn},     // FY04A
  
    {80,31,24,1,evActLedOn},     // PL01A
    {80,32,17,1,evActLedOn},     // PL02A
    {80,33,18,1,evActLedOn},     // PL03A
    {80,34,20,1,evActLedOn},     // HS01AM
    {80,35,20,1,evActLedOn},     // HS01AG                     
            
    {80,41,24,1,evActLedOn},     // G2M
    {80,42,17,1,evActLedOn},     // M2G
    {80,43,18,1,evActLedOn},     // MS
    {80,44,20,1,evActLedOn},     // FY04A         
};      


#endif

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

BYTE findHardCodedEvent( WORD eventNode, WORD eventNum  )

{
    BYTE    i;
    BYTE    eventIndex;
    
    eventIndex = 0xFF;
    
    for (i=0;i<HARDCODED_MAX_LED*HARDCODED_LED_STATES;i++)
        if ((HardCodedEvents[i].evNodeNum == eventNode) && (HardCodedEvents[i].evEventNum == eventNum))
            eventIndex = i;
    
    return( eventIndex);
}

BOOL processHardCodedEvent( WORD eventNode, WORD eventNum, BYTE eventIndex, BYTE *msg )

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
                flashLed(HardCodedEvents[eventIndex].ledNumber);
                clearhardCodedLeds(HardCodedEvents[eventIndex].groupId);
            }    
            break;
            
        case evActLedOn:
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
            
        default:
            eventProcessed = FALSE;
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






