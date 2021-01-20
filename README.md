# CANPanel
MERG CBUS model railway control panel module for PIC18

Currently under development.

The current firmware works on CBUS.

The 64 push buttons create unique long events, and NVs control which are on only, on/off or toggle.

You can set the NVs but there is no event teaching support yet, so you have to hard code the LED response to each incoming event.

The next big step will be to integrate with CBUSLIB to provide full event teaching support.

Pete Brownlow
