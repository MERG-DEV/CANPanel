
/*

 Copyright (C) Pete Brownlow 2018   software@upsys.co.uk

 LED event mapping table for KMRS station

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
	
 * Kingsway Model Railway Society (KMRS) station panel event to LEDs mapping table
 

 */
#pragma code APP
#pragma romdata

#include "hardCoded.h"

// Don't forget to update HARDCODED_MAX_LED in matching header .h file if making changes here!!

const rom HCEvTable HardCodedEvents[] = 
{     
  //    Feedback events from turnouts that set LED on steady - 

  // Node Number, event number, LED number, group number, action

    {1306,102,42,1,evActLedFollow},     // FB Branch station loop reversed
    {1306,102,40,1,evActLedFollowInv},     // FB Branch station loop normal

    {1306,101,20,1,evActLedFollow},     // FB Branch xo reversed
    {1306,101,10,1,evActLedFollowInv},     // FB Branch xo normal
    {1306,104,43,1,evActLedFollowInv},     // FB Branch xo normal


    {1308,104,49,1,evActLedFollow},    // FB P4 down arrival
//    {80,13,1,4,evActLedOn},    //  dummy led always out
    {1308,102,26,1,evActLedFollow},     // FB P3 down arrival
    {1308,102,51,1,evActLedFollowInv},     // FB P3 down arrival inverted
   
    {1308,103,41,1,evActLedFollow},     // FB Shed Entry reversed
    {1308,103,39,1,evActLedFollowInv},     // FB Shed entry normal
    
    
    {1308,204,61,1,evActLedFollowInv},     // Siding-Branch xover reversed
    {1308,204,59,1,evActLedFollow},     // Siding-Branch xover normal
    {1308,105,9,evActLedFollowInv},     // Down-Up  bridge xover normal
    
    {1308,106,28,1,evActLedFollow},     // Down-bay xover reversed
    {1308,106,48,1,evActLedFollowInv},     // Down-bay xover normal
    {1308,107,16,evActLedFollowInv},     // Bay-Down xover normal
 
    
    {1309,102,60,1,evActLedFollowInv},     // FB Up to down xover reversed
    {1309,103,27,1,evActLedFollow},     // FB Down to up xover normal

    {1308,105,61,1,evActLedFollow},     // Siding-Branch xover reversed
    {1308,105,59,1,evActLedFollowInv},     // Siding-Branch xover normal
    {1308,105,9,1, evActLedFollowInv},     // branch siding xover normal
 
    {1309,107,12,1,evActLedFollow},     // FB Up to down xover reversed
    {1309,107,58,1,evActLedFollowInv},     // FB Down to up xover normal
    {1309,108,63,1,evActLedFollowInv},     // FB Down to up xover normal
    
    
    {1313,301,15,1,evActLedFollowInv},     // Platform 1 to 2 boiler end (up) crossover normal
    {1313,301,62,1,evActLedFollow},     // Platform 1 to 2 boiler end (up)crossover reversed
    {1313,302,47,1,evActLedFollow},     // Platform 2 to 1 boiler end (up) crossover normal
 
    {1313,303,18,1,evActLedFollowInv},     // Bay xover normal
    {1313,303,37,1,evActLedFollow},     // Bay/cattle dock xover reversed
    {1313,304,57,1,evActLedFollowInv},     // Cattle dock xover normal
    
    
    {1313,305,33,1,evActLedFollow},     // Platform 3 departure
    {1313,305,32,1,evActLedFollowInv},     // Platform 3 arrival
    

    {1401,80,55,1,evActLedFollow},     // Platform 1 to 2 window end (down) crossover normal from PL2 dep
    {1401,80,45,1,evActLedFollowInv},     // Platform 1 to 2 window end (down)crossover reversed from PL2 dep
    {1401,80,35,1,evActLedFollow},     // Platform 2 to 1 window end (down) crossover normal from PL2 dep
   
    {1401,86,55,1,evActLedFollowInv},     // Platform 1 to 2 window end (down) crossover normal
    {1401,86,45,1,evActLedFollow},     // Platform 1 to 2 window end (down)crossover reversed
    {1401,86,35,1,evActLedFollowInv},     // Platform 2 to 1 window end (down) crossover normal

    {1304,301,56,1,evActLedFollowInv},     // Up-down bridge xover normal
    {1304,301,30,1,evActLedFollow},     // Up-down bridge xover reversed
    {1304,303,64,1,evActLedFollowInv},     // Down-Up  bridge xover normal
 
    {1304,304,32,1,evActLedFollow},     // P3 boiler end arrival
    {1304,304,34,1,evActLedFollowInv},     // P2 boiler end arrival
  
    {1304,302,24,1,evActLedFollow},     // P4 boiler end departure
    {1304,302,33,1,evActLedFollowInv},     // P3 boiler end departure

    {1306,11,25,1,evActLedFollow},     // Overload FY
    {1306,12,8,1,evActLedFollow},     // Overload Main
    {1306,13,17,1,evActLedFollow},     // Overload Branch
  
    
    {1401,5,4,2,evActLedOn},   // Boiler siding 1 
    {1401,7,6,2,evActLedOn},    // Boiler siding 2
    {1401,39,22,2,evActLedOn},   // Boiler siding 3

    
    {80,20,1,0xFF,evNextLed}    // test event to light each LED in turn 
  
}; 
