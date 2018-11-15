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
#include "burnden.h"

// Local prototypes


#pragma code APP
#pragma udata

// Array of buttons and their LEDs and feedbacks 

BYTE    BurndenButtons[BURNDEN_MAX_BUTTON] = {80,96,112,65,81,97,82,98,67,83,99.84,69,101,70,86};
BYTE    ledNum;
//{
//    { 80,0,5}, 
//    { 96,0,6}, 
//    { 112,0,7}, 
//    { 65,1,4}, 
//    { 81,1,5}, 
//    { 97,2,6}, 
//    { 82,2,5}, 
//    { 98,2,6}, 
//    { 67,3,4}, 
//    { 83,3,5}, 
//    { 99,3,6}, 
//    { 84,4,5}, 
//    { 69,5,4}, 
//    { 101,5,6}, 
//    { 70,6,4}, 
//    { 86,6,5}
//};          

// eventsInit called during initialisation - initialises event support 

void BurndenInit(void)

{
    ledNum = 0;
} // eventsInit

void burndenFlashSelected( BYTE button )
{
    int aIndex;
    
    setLed(ledNum++, TRUE);
//    for (aIndex = 0; (aIndex < BURNDEN_MAX_BUTTON) ; aIndex++)
//        if (BurndenButtons[aIndex] == button)
//            setLed(aIndex+32, TRUE);
}




BOOL parseBurndenEvent( BYTE *msg )

{
    overlay BOOL    cmdProcessed = TRUE;
    BYTE            eventIndex;
    BOOL            foundEvent;


    if (foundEvent = ((msg[d0] & EVENT_SET_MASK) == EVENT_SET_MASK) && ((msg[d0] & EVENT_CLR_MASK == 0) ))  // Check correct bits in opcode set or clear for an event
    {
        if ((eventIndex = findEvent(msg[d1]<<8+msg[d2],msg[d3]<<8+msg[d4], FALSE)) != 0)
        {
            processEvent( eventIndex, msg );
        }
    }
    return( foundEvent);
        
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
} 





