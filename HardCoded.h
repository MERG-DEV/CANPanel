#ifndef __HARDCODED_H
#define __HARDCODED_H

/*

 Copyright (C) Pete Brownlow 2014-2017   software@upsys.co.uk

events.h - Definitions for CBUS event handling 
 
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

*/

// #include "cbus.h"
// #include "romops.h"
// #include "EEPROM.h"
#include "max6951.h"


#ifdef KFY

#define HARDCODED_MAX_BUTTON  16            // 16 BUTTONS
#define HARDCODED_MAX_LED     31
#define HARDCODED_LED_STATES    2           // Flashing and steady
#define LED_GROUPS  4                       // Number of groups of mutually exclusive route LEDs

#elif defined KSIGNALS

#define HARDCODED_MAX_BUTTON  16            // 16 BUTTONS
#define HARDCODED_MAX_LED     50           // Number of LED consumer event definitions implemeneted
#define HARDCODED_LED_STATES    1           // Steady (on or off)
#define LED_GROUPS  4                       // Number of groups of mutually exclusive route LEDs

#elif defined KSIGNALMAN

#define HARDCODED_MAX_BUTTON  16            // 16 BUTTONS
#define HARDCODED_MAX_LED     49          // Number of LED consumer event definitions implemeneted
#define HARDCODED_LED_STATES    1           // Steady (on or off)
#define LED_GROUPS  4                       /

#elif defined MSM

#define HARDCODED_MAX_BUTTON  22            // Number of BUTTONS
#define HARDCODED_MAX_LED     22            // Number of LEDs
#define HARDCODED_LED_STATES    2           // Flashing and steady
#define LED_GROUPS  6                       // Number of groups of mutually exclusive route LEDs


#elif defined BURNDENPARK

#define HARDCODED_MAX_BUTTON  41            // Number of BUTTONS
#define HARDCODED_MAX_LED     41            // Number of LEDs
#define HARDCODED_LED_STATES    2           // Flashing and steady
#define LED_GROUPS  6                       // Number of groups of mutually exclusive route LEDs

#elif defined BURNDENSHED

#define HARDCODED_MAX_BUTTON  60            // Number of BUTTONS
#define HARDCODED_MAX_LED    60              // Number of LEDs
#define HARDCODED_LED_STATES    2           // Flashing and steady
#define LED_GROUPS  1                       // Number of groups of mutually exclusive route LEDs

#elif defined INGLEBOROUGH

#define HARDCODED_MAX_BUTTON  40            // Number of BUTTONS
#define HARDCODED_MAX_LED     40            // Number of LEDs
#define HARDCODED_LED_STATES    2           // Flashing and steady
#define LED_GROUPS  1                       // Number of groups of mutually exclusive route LEDs
#endif


#define MAX_STORAGE_ROAD    16                // 13 STORAGE ROADS
#define COMPUTE_NODE        80              // Node number for CANCOMPUTE route set events
#define ROUTE_SETUP_TIME    3 * ONE_SECOND  // Route setup time in seconds when using simulated feedback

typedef struct
{
    BYTE                buttonId;
    BYTE                ledRow;        // Corresponding LED for this button
    BYTE                ledColumn;
} HardCodedEntry;

// Data structure for hard coded simple event table

typedef enum
{
    evActLedOff = 0,
    evActLedOn,
    evActFlashLed,
    evActLedFollow,
    evActLedFollowInv,
    evNextLed
} evActions;

typedef enum
{
    bfNone = 0,
    bfFollow,
    bfFollowInv
} buFollowing;   // Allows button toggle status to follow state of incoming events

typedef struct
{
    WORD        evNodeNum;
    WORD        evEventNum;
    BYTE        ledNumber;
    BYTE        groupId;
    evActions   ledAction;
    buFollowing buttonFollow;
    BYTE        followButton;  // Button whose toggle state will follow the state of this event (0 for none)
} HCEvTable;



void initHardCoded(void);
BYTE hardCodedProducerMap( BYTE button );
void hardCodedFlashSelected( BYTE button );
void checkWaitingRoutes();
void TurnOnNextLed();
        
#endif	// __HARDCODED_H
