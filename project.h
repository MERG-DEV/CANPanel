#ifndef __PROJECT_H
#define __PROJECT_H

// MERG CBUS firmware for CANPANEL module - project definitions

//      All source code is copyright (C) the author(s) concerned
//      Original code up to version 2 (C) 2009 SPROG DCC  http://www.sprog-dcc.co.uk   sprog@sprog-dcc.co.uk
//      Changes for version 3 (C) 2011 Pete Brownlow      merg@uspys.co.uk
//      Support for FLiM, shuttles, DCC accessories and other changes for ver 4 (C) 2011-2012 Pete Brownlow  merg@upsys.co.uk

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, version 3 of the License, as set out
//   at <http://www.gnu.org/licenses/>.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//   See the GNU General Public License for more details.
//
//   As set out in the GNU General Public License, you must retain and acknowledge
//   the statements above relating to copyright and licensing. You must also
//   state clearly any modifications made.  Please therefore retain this header
//   and add documentation of any changes you make. If you distribute a changed
//   version, you must make those changes publicly available.
//
//   The GNU license requires that if you distribute this software, changed or
//   unchanged, or software which includes code from this software, including
//   the supply of hardware that incorporates this software, you MUST either
//   include the source code or a link to a location where you make the source
//   publicly available. The best way to make your changes publicly available is
//   via the MERG online resources.  See <www.merg.co.uk>
//
//   Note that this software uses a Boot Loader that is derived from that supplied
//   by Microchip. That part of the software is distributed under the Microchip
//   license which requires that it is only used on Microchip hardware.
//   See page 15 of <http://ww1.microchip.com/downloads/en/AppNotes/00247a.pdf>
//   for details of the Microchip licensing.  The bootloader is distributed with
//   this software as a "system library" as defined in section 1 of the GNU Public
//   license and is not licensed under GNU.
//   You must conform to Microchip license terms in respect of the bootloader.
//

// This source supports several different hardware configurations.
// The hardware to build for is defined on the command line using -D switch - set this as macro option in MPLAB




// The device to be used

// #ifndef CANkDISP
//  #ifndef CANDISP
            No hardware defined - set command line switch -D<hardwaretype> in project build options
//  #endif
//#endif

// #ifdef CANDISP
//    #include <p18f2580.h>
//    #define CPU P18F2580
//#elif CANKDISP
    #include <p18f25k80.h>
    #define CPU P18F25k80
// #endif

#include <p18f25k80.h>
#define CPU P18F25k80

#include <GenericTypeDefs.h>
#include io-canpanel.h


                    


//
//#include "cbus.h"
//
//// CANCMD headers
//#include "cancmd.h"
//#include "cmdFLiM.h"
//#include "commands.h"
//#include "power_cmds.h"
//#include "program_cmds.h"
//#include "mode_cmds.h"
//#include "packet_gen.h"


//#include "romops.h"
//#include "cbusxdefs.h"
//#include "isr_high.h"



#define MANU_ID         MANU_MERG
#define MODULE_ID       MTYP_CANPANEL
#define MODULE_TYPE     "CANPANEL"

#define MAJOR_VER 	101
#define MINOR_VER 	'a'        // Minor version character
#define MODULE_FLAGS    0b00001011  // Producer, consumer, boot
#define BUS_TYPE        PB_CAN
#define LOAD_ADDRESS    0x0800      // Need to put in parameter block at compile time, only known to linker so hard code here
#define MNAME_ADDRESS   LOAD_ADDRESS + 0x20 + sizeof(ParamBlock)   // Put module type string above params so checksum can be calculated at compile time
                    
#define TEST_VER        1
#define BUILD_NO        1

// DEFINE DEFAULT VALUES

#define HEARTBEAT	500000	// mS for heartbeat interrupt






// Reload TMR0 for interrupt every 2mS
// Tins = 62.5ns @ 64MHz
// Use 1:12864 prescaler, timer ticks every 8uS (Ttmr)
// Interrupt every 250 Ttmr
// Value loaded into TMR0 needs to be adjusted for:
// - TMR0 interrupt latency (3 Tins)
// - Number of instructions from interrupt to reload
// - TMR0 inhibit after reload (2 Tins)
// With Prescaler = 128:1, effect of all of these is 0 to 1 Ttmr (mostly 0), so no adjustment required
// So value is not 0 - (250 - 3 - 30 - 2)/4 = -107 = 0x95

#define TMR0_INTERVAL   .250
                    



#endif	// __PROJECT_H
