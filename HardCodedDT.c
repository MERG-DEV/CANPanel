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

#define RUN_LED1 26  // LED number to light when up and running
#define RUN_LED2 30  // LED number to light when up and running
#define RUN_LED3 27  // LED number to light when up and running
#define RUN_LED4 31  // LED number to light when up and running





const rom BYTE    hardCodedButtons[HARDCODED_MAX_BUTTON] = {64,80,96,112,81,97,82,98,84,100,69,101,70,86,65,66};
const rom BYTE    hardCodedLeds[HARDCODED_MAX_BUTTON] =    {48,56,64 ,33,41,49,42,50,35,43,51,44,45,53 ,36,37};
const rom BYTE    ledGroupLimits[LED_GROUPS+1] = {1,10,20,30};

// Dont forget to update HARDCODED_MAX_LED in hardcoded.h if making changes here!!

const rom HCEvTable HardCodedEvents[] = 
{   
#ifdef KFY
//   Button events that start LED flashing - Node Number, event number, LED number, Group, action
    {450,64,25,0,evActFlashLed},   // FYUPBH correct
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

// Dont forget to update HARDCODED_MAX_LED in hardcoded.h if making changes here!!
#elif defined KSIGNALS
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action
    {431,9,58,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,59,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,56,0xFF,evActLedFollowInv},     // FB: Up Starter Clear
    {432,10,49,0xFF,evActLedFollowInv},    // FB: Up Starter Danger      
    {432,11,60,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,61,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,42,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,43,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,45,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,46,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,16,50,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,15,51,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,13,52,0xFF,evActLedFollowInv},     // FB: MPD Exit Clear       
    {435,14,53,0xFF,evActLedFollowInv},     // FB: MPD Exit Danger

    {436,16,54,0xFF,evActLedFollowInv},     // FB: Up Home Clear       
    {436,15,55,0xFF,evActLedFollowInv},     // FB: Up Home Danger
    {435,12,48,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Clear       
    {435,11,41,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Danger   
    {431,11,24,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {431,12,17,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger   
    {432,14,32,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {432,13,25,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger              
    {432,16,29,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Clear       
    {432,15,30,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Danger               
    {433,14,40,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Clear       
    {433,13,33,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Danger               
            
//    {452,103,9,0xFF,evActLedFollow},    // Emergency stop all
    {406,11,15,0xFF,evActLedFollowInv},     // 19A indicator
    {406,11,14,0xFF,evActLedFollow},     // 19A reverse indicator
    {406,9,13,0xFF,evActLedFollow},     // 19B normal indicator
    {406,10,10,0xFF,evActLedFollowInv},     // 18A indicator
    {406,10,11,0xFF,evActLedFollow},     // 18A reverse indicator
    {407,11,12,0xFF,evActLedFollowInv},     // 18B indicator
           
    {407,10,9,0xFF,evActLedFollow},     // 27A normal 
    {407,9,16,0xFF,evActLedFollowInv},     // 27B reversed indicator
    {407,9,7,0xFF,evActLedFollow},     // 27B normal indicator            
    {408,10,5,0xFF,evActLedFollow},     // 15A normal indicator
    {408,10,4,0xFF,evActLedFollowInv},     // 15A reverse indicator
    {408,9,3,0xFF,evActLedFollowInv},     // 15B normal indicator
    {409,9,8,0xFF,evActLedFollowInv},  // 8 normal indicator
    {409,9,1,0xFF,evActLedFollow},  // 8 Reverse indicator
    {410,9,2,0xFF,evActLedFollowInv},     // 9 normal indicator
    {410,10,6,0xFF,evActLedFollow},   // 10 reverse indicator
            
//  Test events for finding out which LEDs are which - leave commented out for normal use
            
//    {80,20,1,0xFF,evNextLed}   
};

#elif defined KSIGNALMAN
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action
    {431,9,60,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,61,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,48,0xFF,evActLedFollowInv},     // FB: Up Starter Clear
    {432,10,41,0xFF,evActLedFollowInv},    // FB: Up Starter Danger      
    {432,11,64,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,57,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,40,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,33,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,50,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,51,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,16,56,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,15,49,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,13,59,0xFF,evActLedFollow},     // FB: MPD Exit Clear       
    {435,14,58,0xFF,evActLedFollow},     // FB: MPD Exit Danger
//    {452,103,9,0xFF,evActLedFollow},    // Emergency stop all
    {406,11,63,0xFF,evActLedFollowInv},     // 19A normal indicator
    {406,11,30,0xFF,evActLedFollow},     // 19A reverse indicator
    {406,9,47,0xFF,evActLedFollow},     // 19B normal indicator
    {406,10,31,0xFF,evActLedFollowInv},     // 18A indicator
    {406,10,38,0xFF,evActLedFollow},     // 18A reverse indicator
    {407,11,21,0xFF,evActLedFollowInv},     // 18B indicator
//            
//    {407,11,28,0xFF,evActLedFollowInv},     // 18B indicator
//    {407,11,63,0xFF,evActLedFollowInv},     // 18B alt indicator
//            
    {407,10,29,0xFF,evActLedFollow},     // 27A normal indicator 
    {407,9,5,0xFF,evActLedFollow},     // 27B reversed indicator
    {407,9,54,0xFF,evActLedFollowInv},     // 27B normal indicator          
    {408,10,7,0xFF,evActLedFollow},     // 15A normal indicator
    {408,10,14,0xFF,evActLedFollowInv},     // 15A reverse indicator
    {408,9,39,0xFF,evActLedFollowInv},     // 15B normal indicator
    {409,9,23,0xFF,evActLedFollowInv},  // 8 normal indicator
    {409,9,46,0xFF,evActLedFollow},  // 8 Reverse indicator
    {410,9,13,0xFF,evActLedFollowInv},     // 9 normal indicator
    {410,10,6,0xFF,evActLedFollow},   // 10 reverse indicator
    {452,39,22,0xFF,evActLedFollow},   // Stop all indicator
            
    {431,11,2,0xFF,evActLedFollow},   // FB: Shunt from Up line 12 Danger
    {431,12,1,0xFF,evActLedFollow},   // FB: Shunt from Up line 12 Clear     
    {432,13,26,0xFF,evActLedFollow},   // FB: Shunt from Bay Clear 
    {432,14,18,0xFF,evActLedFollow},   // FB: Shunt from Bay Danger  
    {432,15,16,0xFF,evActLedFollow},   // FB: Shunt Dn Plat to Up Clear  
    {432,16,8,0xFF,evActLedFollow},   // FB: Shunt Dn Plat to Up Danger  
    {433,13,34,0xFF,evActLedFollow},   // FB: Shunt Dn Plat to Goods Clear 
    {433,14,35,0xFF,evActLedFollow},   // FB: Shunt Dn Plat to Goods Danger  
    {435,11,36,0xFF,evActLedFollow},   // FB: Up Refuge Starter Clear
    {435,12,37,0xFF,evActLedFollow},   // FB: Up Refuge Starter Danger
    {436,15,42,0xFF,evActLedFollow},   // FB: FB Up Home Clear
    {436,16,43,0xFF,evActLedFollow},   // FB: FB Up Home Clear
            
    {400,11,19,0xFF,evActLedFollow},   // Up Yard Short
    {400,12,27,0xFF,evActLedFollow},   // Down Yard Short 
    {411,9,11,0xFF,evActLedFollow},   // Down Main Short    
    {411,10,3,0xFF,evActLedFollow},   // Up Main Short    
    {411,11,20,0xFF,evActLedFollow},   // Brewery Short    
    {411,12,12,0xFF,evActLedFollow},   // MPD Short    
     
//  Test events for finding out which LEDs are which - leave commented out for normal use
            
};  

// Dont forget to update HARDCODED_MAX_LED in hardcoded.h if making changes here!!
#elif defined BURNDENPARK
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action, button number to follow event state
    {431,9,58,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,59,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,56,0xFF,evActLedFollowInv},     // FB: Up Starter Clear
    {432,10,49,0xFF,evActLedFollowInv},    // FB: Up Starter Danger      
    {432,11,60,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,61,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,42,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,43,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,45,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,46,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,16,50,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,15,51,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,13,52,0xFF,evActLedFollowInv},     // FB: MPD Exit Clear       
    {435,14,53,0xFF,evActLedFollowInv},     // FB: MPD Exit Danger

    {436,16,54,0xFF,evActLedFollowInv},     // FB: Up Home Clear       
    {436,15,55,0xFF,evActLedFollowInv},     // FB: Up Home Danger
    {435,12,48,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Clear       
    {435,11,41,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Danger   
    {431,11,24,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {431,12,17,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger   
    {432,14,32,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {432,13,25,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger              
    {432,16,29,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Clear       
    {432,15,30,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Danger               
    {433,14,40,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Clear       
    {433,13,33,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Danger               
            
//    {452,103,9,0xFF,evActLedFollow},    // Emergency stop all
    {406,11,15,0xFF,evActLedFollowInv},     // 19A indicator
    {406,11,14,0xFF,evActLedFollow},     // 19A reverse indicator
    {406,9,13,0xFF,evActLedFollow},     // 19B normal indicator
    {406,10,10,0xFF,evActLedFollowInv},     // 18A indicator
    {406,10,11,0xFF,evActLedFollow},     // 18A reverse indicator
    {407,11,12,0xFF,evActLedFollowInv},     // 18B indicator
           
    {407,10,9,0xFF,evActLedFollow},     // 27A normal 
    {407,9,16,0xFF,evActLedFollowInv},     // 27B reversed indicator
    {407,9,7,0xFF,evActLedFollow},     // 27B normal indicator            
    {408,10,5,0xFF,evActLedFollow},     // 15A normal indicator
    {408,10,4,0xFF,evActLedFollowInv},     // 15A reverse indicator
    {408,9,3,0xFF,evActLedFollowInv},     // 15B normal indicator
    {409,9,8,0xFF,evActLedFollowInv},  // 8 normal indicator
    {409,9,1,0xFF,evActLedFollow},  // 8 Reverse indicator
    {410,9,2,0xFF,evActLedFollowInv},     // 9 normal indicator
    {410,10,6,0xFF,evActLedFollow},   // 10 reverse indicator
            
//  Test events for finding out which LEDs are which - leave commented out for normal use
            
    {80,20,1,0xFF,evNextLed}   
};

// Dont forget to update HARDCODED_MAX_LED in hardcoded.h if making changes here!!
#elif defined BURNDENSHED
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action
    {431,9,58,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,59,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,56,0xFF,evActLedFollowInv},     // FB: Up Starter Clear
    {432,10,49,0xFF,evActLedFollowInv},    // FB: Up Starter Danger      
    {432,11,60,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,61,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,42,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,43,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,45,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,46,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,16,50,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,15,51,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,13,52,0xFF,evActLedFollowInv},     // FB: MPD Exit Clear       
    {435,14,53,0xFF,evActLedFollowInv},     // FB: MPD Exit Danger

    {436,16,54,0xFF,evActLedFollowInv},     // FB: Up Home Clear       
    {436,15,55,0xFF,evActLedFollowInv},     // FB: Up Home Danger
    {435,12,48,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Clear       
    {435,11,41,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Danger   
    {431,11,24,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {431,12,17,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger   
    {432,14,32,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {432,13,25,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger              
    {432,16,29,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Clear       
    {432,15,30,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Danger               
    {433,14,40,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Clear       
    {433,13,33,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Danger               
            
//    {452,103,9,0xFF,evActLedFollow},    // Emergency stop all
    {406,11,15,0xFF,evActLedFollowInv},     // 19A indicator
    {406,11,14,0xFF,evActLedFollow},     // 19A reverse indicator
    {406,9,13,0xFF,evActLedFollow},     // 19B normal indicator
    {406,10,10,0xFF,evActLedFollowInv},     // 18A indicator
    {406,10,11,0xFF,evActLedFollow},     // 18A reverse indicator
    {407,11,12,0xFF,evActLedFollowInv},     // 18B indicator
           
    {407,10,9,0xFF,evActLedFollow},     // 27A normal 
    {407,9,16,0xFF,evActLedFollowInv},     // 27B reversed indicator
    {407,9,7,0xFF,evActLedFollow},     // 27B normal indicator            
    {408,10,5,0xFF,evActLedFollow},     // 15A normal indicator
    {408,10,4,0xFF,evActLedFollowInv},     // 15A reverse indicator
    {408,9,3,0xFF,evActLedFollowInv},     // 15B normal indicator
    {409,9,8,0xFF,evActLedFollowInv},  // 8 normal indicator
    {409,9,1,0xFF,evActLedFollow},  // 8 Reverse indicator
    {410,9,2,0xFF,evActLedFollowInv},     // 9 normal indicator
    {410,10,6,0xFF,evActLedFollow},   // 10 reverse indicator
            
//  Test events for finding out which LEDs are which - leave commented out for normal use
            
//    {80,20,1,0xFF,evNextLed}   
};

// Dont forget to update HARDCODED_MAX_LED in hardcoded.h if making changes here!!
#elif defined INGLEBOROUGH
//   Signal feedback events that set LEDs on panel - Node Number, event number, LED number, Group, action
    {431,9,58,0xFF,evActLedFollow},     // FB: Down Home Clear
    {431,10,59,0xFF,evActLedFollow},    // FB: Down Home Danger
    {432,9,56,0xFF,evActLedFollowInv},     // FB: Up Starter Clear
    {432,10,49,0xFF,evActLedFollowInv},    // FB: Up Starter Danger      
    {432,11,60,0xFF,evActLedFollow},    // FB: Goods Loop Starter Clear
    {432,12,61,0xFF,evActLedFollow},    // FB: Goods Loop Starter Danger    
    {434,9,42,0xFF,evActLedFollow},     // FB: Up Outer Home Clear
    {434,10,43,0xFF,evActLedFollow},    // FB: Up Outer Home Danger      
    {434,11,45,0xFF,evActLedFollow},    // FB: Down Advanced Starter Clear
    {434,12,46,0xFF,evActLedFollow},     // FB: Down Advanced Starter Danger
    {435,16,50,0xFF,evActLedFollow},     // FB: Down Starter Clear       
    {435,15,51,0xFF,evActLedFollow},     // FB: Down Starter Danger
    {435,13,52,0xFF,evActLedFollowInv},     // FB: MPD Exit Clear       
    {435,14,53,0xFF,evActLedFollowInv},     // FB: MPD Exit Danger

    {436,16,54,0xFF,evActLedFollowInv},     // FB: Up Home Clear       
    {436,15,55,0xFF,evActLedFollowInv},     // FB: Up Home Danger
    {435,12,48,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Clear       
    {435,11,41,0xFF,evActLedFollowInv},     // FB: Up Refuge Starter Danger   
    {431,11,24,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {431,12,17,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger   
    {432,14,32,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Clear       
    {432,13,25,0xFF,evActLedFollowInv},     // FB: Up Shunt 12 Danger              
    {432,16,29,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Clear       
    {432,15,30,0xFF,evActLedFollowInv},     // FB: 11 Shunt down platform to Up Danger               
    {433,14,40,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Clear       
    {433,13,33,0xFF,evActLedFollowInv},     // FB: 16 Shunt down platform to Goods Danger               
            
//    {452,103,9,0xFF,evActLedFollow},    // Emergency stop all
    {406,11,15,0xFF,evActLedFollowInv},     // 19A indicator
    {406,11,14,0xFF,evActLedFollow},     // 19A reverse indicator
    {406,9,13,0xFF,evActLedFollow},     // 19B normal indicator
    {406,10,10,0xFF,evActLedFollowInv},     // 18A indicator
    {406,10,11,0xFF,evActLedFollow},     // 18A reverse indicator
    {407,11,12,0xFF,evActLedFollowInv},     // 18B indicator
           
    {407,10,9,0xFF,evActLedFollow},     // 27A normal 
    {407,9,16,0xFF,evActLedFollowInv},     // 27B reversed indicator
    {407,9,7,0xFF,evActLedFollow},     // 27B normal indicator            
    {408,10,5,0xFF,evActLedFollow},     // 15A normal indicator
    {408,10,4,0xFF,evActLedFollowInv},     // 15A reverse indicator
    {408,9,3,0xFF,evActLedFollowInv},     // 15B normal indicator
    {409,9,8,0xFF,evActLedFollowInv},  // 8 normal indicator
    {409,9,1,0xFF,evActLedFollow},  // 8 Reverse indicator
    {410,9,2,0xFF,evActLedFollowInv},     // 9 normal indicator
    {410,10,6,0xFF,evActLedFollow},   // 10 reverse indicator
            
//  Test events for finding out which LEDs are which - leave commented out for normal use
            
//    {80,20,1,0xFF,evNextLed}   
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
            
        default:
            eventProcessed = FALSE;
    }
   
    if (HardCodedEvents[eventIndex].followButton != 0)
    {
        setButtonState( BYTE HardCodedEvents[eventIndex].followButton, BOOL onEvent );
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






