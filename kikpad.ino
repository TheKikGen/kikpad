/*
__ __| |           |  /_) |     ___|             |           |
   |   __ \   _ \  ' /  | |  / |      _ \ __ \   |      _` | __ \   __|
   |   | | |  __/  . \  |   <  |   |  __/ |   |  |     (   | |   |\__ \
  _|  _| |_|\___| _|\_\_|_|\_\\____|\___|_|  _| _____|\__,_|_.__/ ____/
  -----------------------------------------------------------------------------
  KIKPAD  - Alternative firmware for the Midiplus Smartpad.
  Copyright (C) 2020 by The KikGen labs.
  LICENCE CREATIVE COMMONS - Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)

  This file is part of the KIKPAD distribution
  https://github.com/TheKikGen/kikpad
  Copyright (c) 2020 TheKikGen Labs team.
  -----------------------------------------------------------------------------
  Disclaimer.

  This work is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License.
  To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/4.0/
  or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

  NON COMMERCIAL - PERSONAL USE ONLY : You may not use the material for pure
  commercial closed code solution without the licensor permission.

  You are free to copy and redistribute the material in any medium or format,
  adapt, transform, and build upon the material.

  You must give appropriate credit, a link to the github site
  https://github.com/TheKikGen/USBMidiKliK4x4 , provide a link to the license,
  and indicate if changes were made. You may do so in any reasonable manner,
  but not in any way that suggests the licensor endorses you or your use.

  You may not apply legal terms or technological measures that legally restrict
  others from doing anything the license permits.

  You do not have to comply with the license for elements of the material
  in the public domain or where your use is permitted by an applicable exception
  or limitation.

  No warranties are given. The license may not give you all of the permissions
  necessary for your intended use.  This program is distributed in the hope that
  it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

////////////////////////////////////////////////////////////////////////////////
// KIKPAD SYSTEM KEYS
//------------------------------------------------------------------------------
// RESET             = HOLD BT_CONTROL4 & MASTER8 THEN PRESS SET
// BOOTLOADER MODE   = HOLD BT_CONTROL4 & MASTER7 THEN PRESS SET
////////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdarg.h>

#include <libmaple/nvic.h>
#include "libmaple/flash.h"
#include "libmaple/pwr.h"
#include "libmaple/rcc.h"
#include "libmaple/bkp.h"

#include "kikpad.h"
#include "usb_midi.h"
#include "usb_midi_device.h"
#include "ringbuffer.h"
#include "Rotary.h"
#include <midiXparser.h>

///////////////////////////////////////////////////////////////////////////////
// GLOBALS
///////////////////////////////////////////////////////////////////////////////

// TIMER
HardwareTimer RGBRefreshTim2(2);
HardwareTimer UserEventsTim3(3);

// Queue (ring buffer) for User events
RingBuffer<uint8_t,RB_UEVENT_SIZE> UserEventQueue;

// Multi purpose data buffer.
uint8_t globalDataBuffer[GLOBAL_DATA_BUFF_SIZE] ;

// Serial interfaces Array
HardwareSerial * serialHw = &Serial1;

// USB Midi object & globals
USBMidi MidiUSB;
bool	  midiUSBCx      = false ;
bool    midiUSBIdle    = false ;
bool 	  isSerialBusy   = false ;

// MIDI Parsers for serial 1
midiXparser midiSerial;


// Buttons Pad & bars scan lines
const uint8_t    ScanButtonsRows[]      =  {PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15};
const uint8_t    ScanButtonsColumns[]   =  {PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10};

// Encoders
Rotary SPRotary[8];

// Led bank color map, in the same order than enum LedBankIds
const ledColor_t PadLedBanksColorMap[] = { BLUE,RED,GREEN,BLUE,RED,GREEN,};

// Color RGB quantization masks 3 colors x 6 banks
static volatile uint32_t PadLedBanksRGBMsk[PAD_COLOR_DEPTH][LED_BANK_MAX-2];

// Buttons Banks and mask settings
const uint32_t ButtonLedBankMsk[] = {
  BTMSK_MS1,   BTMSK_MS2,  BTMSK_MS3,  BTMSK_MS4,  BTMSK_MS5,     BTMSK_MS6,     BTMSK_MS7,     BTMSK_MS8,
  BTMSK_UP,    BTMSK_DOWN, BTMSK_LEFT, BTMSK_RIGHT,BTMSK_CLIP,    BTMSK_MODE1,   BTMSK_MODE2,   BTMSK_SET,
  BTMSK_VOLUME,BTMSK_SENDA,BTMSK_SENDB,BTMSK_PAN,  BTMSK_CONTROL1,BTMSK_CONTROL2,BTMSK_CONTROL3,BTMSK_CONTROL4,
};

const uint8_t ButtonLedBankMap[] = {
  1,1,1,1,1,1,1,1,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
};

// Current led states (ON/Off)
// Pads : Higer / Lower
volatile uint32_t  PadLedStates[]        = { LED_BK_PATTERN1, LED_BK_PATTERN1 };
// Buttons : Left, bottom / Right
volatile uint32_t  ButtonsLedStates[]    = { 0, 0 };

// Current pad colors set
uint8_t PadColorsCurrent[] {
   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN,

   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN
};

const uint8_t KikGenLogo[] {
  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,
  WHITE,  BLUE ,  ORANGE, ORANGE, ORANGE, BLUE ,  ORANGE, WHITE,
  WHITE,  BLUE ,  ORANGE, ORANGE, BLUE ,  ORANGE, ORANGE, WHITE,
  WHITE,  BLUE ,  BLUE ,  BLUE ,  ORANGE, ORANGE, ORANGE, WHITE,
  WHITE,  BLUE ,  BLUE ,  BLUE ,  ORANGE, ORANGE, ORANGE, WHITE,
  WHITE,  BLUE ,  ORANGE, ORANGE, BLUE ,  ORANGE, ORANGE, WHITE,
  WHITE,  BLUE ,  ORANGE, ORANGE, ORANGE, BLUE ,  ORANGE, WHITE,
  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,  WHITE,
};


// Temporary pad color save
uint8_t PadColorsBackup[PAD_SIZE];

// Pad & buttons scan lines states
volatile uint16_t BtnScanStates[8][11] = {
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
};

///////////////////////////////////////////////////////////////////////////////
//  CODE MODULES
//-----------------------------------------------------------------------------
// Due to the unusual make process of Arduino platform, modules are included
// directly here as "h" type. This allows a better code separation and readability.
///////////////////////////////////////////////////////////////////////////////
// DO NOT REMOVE OR CHANGE THE ORDER !


// Kikpad functionnal module.Uncomment only one.

//#include "mod_kikpad_demo.h"
//#include "mod_kikpad_MPC.h"
//#include "mod_kikpad_MPCClips.h"
//#include "mod_kikpad_MPCController.h"
#include "mod_kikpad_MPCForce.h"


///////////////////////////////////////////////////////////////////////////////
//  CORE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// SerialPrintf : a light printf like to Serial.
// %%  : %
// %n  : new line
// %s  : strings
// %m  : string minor case
// %M  : string major case
// %y  : string beautify (fist letter major case)
// %c  : character
// %x  : hexa
// %d  : integer
// %b  : binary
// %(n)s : print n char from string or char array
// %(nn)d/x/b : pad space left until size n
// %(0n)d/x/b : pad 0 left untile size n (nb for b : n=32 bits max).
///////////////////////////////////////////////////////////////////////////////
#define _PRINT_OUT Serial.print
void SerialPrintf(const char *format, ...)
{
  va_list varg;
  va_start(varg, format);

  while ( *format != 0 ) {
    if (*format == '%') {
      format++;
      if (*format == '%') _PRINT_OUT(*format);
      else if (*format == 'b') _PRINT_OUT(va_arg(varg, long),BIN); // Binary
      else if (*format == 'c') _PRINT_OUT((char)va_arg(varg, int)); // Char (must be casted to int)
      else if (*format == 'd') _PRINT_OUT(va_arg(varg, long ));  // long int
      else if (*format == 'u') _PRINT_OUT(va_arg(varg, unsigned long));  // u long int
      else if (*format == 'x') _PRINT_OUT(va_arg(varg, long),HEX);  // hexa
			else if (*format == 's' || *format == 'M' || *format == 'm' || *format == 'y') {
				char * str=va_arg(varg, char*);
				uint16_t i = 0;
				while (*str) {
						char c = *str;
						if ( c >= 'A' && c <= 'Z' ) {
							if ( *format == 'm' || (*format == 'y' && i > 0) ) c +=  0x20;
						} else
						if ( c >= 'a' && c <= 'z') {
							if ( *format == 'M' || (*format == 'y' && i == 0) ) c -=  0x20;
						}
						_PRINT_OUT(c); str++; i++;
				}
			}
      else if (*format >= '0' && *format <= '9' ) {
        char p =' ';
        if ( *format == '0') { format++; p = '0';}
        if ( *format >='1' && *format <= '9' ) {
          uint8_t pad = *format - '0';
          format++;
          if ( *format >='0' && *format <= '9' ) pad = pad*10 + *(format++) - '0';
					if ( *format == 'c' ) {
						while (--pad) _PRINT_OUT(p);
						_PRINT_OUT((char)va_arg(varg, int));
					}
          else if ( *format == 's' || *format == 'M' || *format == 'm' || *format == 'y') {
            char * str=va_arg(varg, char*);
						uint16_t i = 0;
						while (pad--) {
								if (*str) {
									char c = *str;
									if ( c >= 'A' && c <= 'Z' ) {
										if ( *format == 'm' || (*format == 'y' && i > 0) ) c +=  0x20;
									} else
									if ( c >= 'a' && c <= 'z') {
										if ( *format == 'M' || (*format == 'y' && i == 0) ) c -=  0x20;
									}
									_PRINT_OUT(c); str++; i++;
								}
								else break;
						}
					}
          else {
            uint8_t  base = 0;
            if ( *format == 'd' ) base = 10;
            else if ( *format =='x' ) base = 16;
            else if ( *format =='b' ) {
                base = 2;
                if ( pad > 32 ) pad = 8; // 32 bits max
            }
            if (base) {
							int value = va_arg(varg, long);
              uint32_t pw   = base;
              while (pad--) {
                if ( value < (int)pw ) while (pad) { _PRINT_OUT(p);pad--;}
                else pw *= base;
              }
              if (base == 16) _PRINT_OUT(value,HEX);
              else if (base == 2) _PRINT_OUT(value,BIN);
              else _PRINT_OUT(value);
            }
          }
        } else return;
      }
      else if (*format == 'n') Serial.println(); // new line
      else return;
    } else Serial.print(*format);
    format++;
  }
  va_end(varg);
}
///////////////////////////////////////////////////////////////////////////////
// DMC 13C LED Driver management
//-----------------------------------------------------------------------------
// 2 16 bits DMC 13 are cascaded in the SMARTPAD, so 32 bits can be written.
// Bits are sent serialized from 0 to 31 with a clokc synchronization.
// End of transmission is done by raising LAT(ch)
///////////////////////////////////////////////////////////////////////////////
static void WriteDMC(uint32_t mask) {
  //FAST_DIGITAL_WRITE(DMC_DCK,0);
  //FAST_DIGITAL_WRITE(DMC_LAT,0);
  //delayMicroseconds(DELAY_DMC);
  for ( uint8_t i = 0; i!= 32 ; i++ ) {
  //while (mask) {
    if ( mask & 1 ) FAST_DIGITAL_WRITE(DMC_DAI, 1 );
    else FAST_DIGITAL_WRITE(DMC_DAI, 0 );
    //delayMicroseconds(DELAY_DMC);
    FAST_DIGITAL_WRITE(DMC_DCK,1);
    //delayMicroseconds(DELAY_DMC);
    FAST_DIGITAL_WRITE(DMC_DCK,0);
    //delayMicroseconds(DELAY_DMC);
    mask >>= 1;
  }
  FAST_DIGITAL_WRITE(DMC_LAT,1);
  //delayMicroseconds(DELAY_DMC);
  FAST_DIGITAL_WRITE(DMC_LAT,0);
  //delayMicroseconds(DELAY_DMC);
}

///////////////////////////////////////////////////////////////////////////////
// Operate the LS138 BRG colors LED multiplexer and buttons LEDs
//-----------------------------------------------------------------------------
// 8 address can be set :
// . 0-2 are used by higher pads, respectively for colors Blue, Red, Green
// . 3-5 are used by lower pads, respectively for colors Blue, Red, Green
// . 6-7 are used by buttons left+bottom and right
///////////////////////////////////////////////////////////////////////////////
static boolean LedBankSet(uint8_t addr ) {
  static uint8_t lastAddr = 0xFF;
  if ( addr == lastAddr  ) return false;

  lastAddr = addr;

  if ( addr & 1 ) FAST_DIGITAL_WRITE(LS_A0, 1  ) ;
  else FAST_DIGITAL_WRITE(LS_A0, 0  ) ;

  if ( addr & 2 ) FAST_DIGITAL_WRITE(LS_A1, 1  ) ;
  else FAST_DIGITAL_WRITE(LS_A1, 0  ) ;

  if ( addr & 4 ) FAST_DIGITAL_WRITE(LS_A2, 1  ) ;
  else FAST_DIGITAL_WRITE(LS_A2, 0  ) ;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Update the RGB colors mask for one pad color only
///////////////////////////////////////////////////////////////////////////////
void RGBMaskUpdate(uint8_t padIdx) {
  if (padIdx >= PAD_SIZE) return;
  uint8_t idx,ofs,st;

  if ( padIdx < 32 ) { idx = 31-padIdx; ofs = 0; st = 0; }
  else { idx = 63-padIdx; ofs = 3; st = 1; }

  for ( uint8_t b= ofs ; b != ofs+3 ; b++) {
    for (uint8_t colorQ = 0; colorQ != PAD_COLOR_DEPTH ; colorQ++) {
        uint8_t colorShift = colorQ < 2 ? 0:3;  // Manage msb/lsb
        if (  ( (PadColorsCurrent[padIdx]>>colorShift) & PadLedBanksColorMap[b] ) && (PadLedStates[st] & (1<< idx) ) )
              PadLedBanksRGBMsk[colorQ][b] |= 1<<idx;    // set
        else  PadLedBanksRGBMsk[colorQ][b] &= ~(1<<idx); // reset
    }
  }
}
///////////////////////////////////////////////////////////////////////////////
// Update RGB colors mask used for RGB quantization of all 64 pads
///////////////////////////////////////////////////////////////////////////////
void RGBMaskUpdateAll() {
  for ( uint8_t b = 0 ; b!= 6; b++) {

    // Color Depth
    for (uint8_t colorQ = 0; colorQ != 3 ; colorQ++) {
        uint8_t colorShift = colorQ < 2 ? 0:3;  // Manage msb/lsb
        uint8_t  ofs, idx;

        PadLedBanksRGBMsk[colorQ][b] = 0;
        if ( b < LED_BANK_LPAD_B ) { ofs = 31; idx = 0;}   // Higher pad
        else { ofs = 63; idx = 1; } // Lower pad

        // 32 pad led par bank
        for ( uint8_t i = 0; i != 32 ; i++ ) {
          if ( ( (PadColorsCurrent[ofs - i]>>colorShift) & PadLedBanksColorMap[b] )
                         && (PadLedStates[idx] & (1<<i) ) ) PadLedBanksRGBMsk[colorQ][b] |= 1<<i;
        }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// CRTICAL ISR - TIMER2. USed for RGB colors quantizatiton.
//-----------------------------------------------------------------------------
// 64 colors are generated (3 colors depth) - 8 bits xxrgbRGB (EGA)
// Buttons leds are also refreshed here (on/off only)
// Duration : Around 60 uS
///////////////////////////////////////////////////////////////////////////////
void RGBTim2Handler() {
  static uint8_t ledPadBk = 0;
  static uint8_t ledBtBk  = 0;
  static uint8_t colorQ   = 0;

  // LEDs /////////////////////////////////////////////////////////////////////
  // Ligh off last bank.
  WriteDMC(0);

  // Pads leds
  if ( ledPadBk < LED_BANK_BT1 ) {
      LedBankSet(ledPadBk);
      WriteDMC(PadLedBanksRGBMsk[colorQ][ledPadBk]);
      ledPadBk++;
  }

  // Alternate 3 colors with buttons led bank to equilibrate timings between colors
  else {
      ledPadBk = 0;
      if ( ++colorQ == 3 ) colorQ = 0;
      LedBankSet(LED_BANK_BT1 + ledBtBk);
      WriteDMC(ButtonsLedStates[ledBtBk]);
      ledBtBk = ! ledBtBk; // 2 buttons banks only, so we can toggle
  }
}

///////////////////////////////////////////////////////////////////////////////
// CRTICAL ISR - TIMER3. USed for encoders and buttons scan lines
//-----------------------------------------------------------------------------
// Encoders are multiplexed with an LS138 (8 address for 8 encoders), so
// keeping encoders reactives is a bit tricky.
// Buttons scan lines : 8 rows / 11 columns.
// Pads are 0,0 - 0,7
// Columns 8,9, 10 are buttons right, bottom, left
// Duration : around 25 uS without events
///////////////////////////////////////////////////////////////////////////////
void UserEventsTim3Handler() {
  static uint8_t ecAddr = 0;

  // Encoders /////////////////////////////////////////////////////////////////

  // Set LS138 encoder address
  if ( ecAddr & 1 ) FAST_DIGITAL_WRITE(EC_LS_A0, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A0, 0  ) ;

  if ( ecAddr & 2 ) FAST_DIGITAL_WRITE(EC_LS_A1, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A1, 0  ) ;

  if ( ecAddr & 4 ) FAST_DIGITAL_WRITE(EC_LS_A2, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A2, 0  ) ;
  // mandatory delay to debounce
  delayMicroseconds(4);

  // Read the direction or nothing...
  uint8_t state = SPRotary[ecAddr].read();
  if (state == Rotary::dirCw ) {
        // Simulate a line 0, col = addr
       UserEvent_t ev = { .ev = EV_EC_CW, .d1 = 0 , .d2 = ecAddr } ;
       UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));

  } else if (state == Rotary::dirCCw ) {
      UserEvent_t ev = { .ev = EV_EC_CCW, .d1 = 0, .d2 = ecAddr } ;
      UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
  }

  if (++ecAddr == 8) ecAddr = 0;

  // Scan lines ////////////////////////////////////////////////////////////////
  static uint8_t c = 0;
    //for ( uint8_t c = 0 ; c!= sizeof(ScanButtonsColumns); c++ ) {
    for ( uint8_t r = 0 ; r !=sizeof(ScanButtonsRows) ;r++  ) {
          FAST_DIGITAL_WRITE(ScanButtonsRows[r],1);
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],0);
          if ( FAST_DIGITAL_READ(ScanButtonsRows[r]) == 0 ) {
            if (BtnScanStates[r][c] == 0 ) {
                BtnScanStates[r][c] = 1;
                UserEvent_t ev;
                // NB : scan rows are columns on the pad matrix !
                // So to match the visual matrix, r and c are inversed.
                // The same for buttons, more convenient with 8 columns instead
                // of 8 lines/3 columns. Better match with pads.
                if ( c < 8 ) ev = { .ev = EV_PAD_PRESSED, .d1 = c, .d2 = r } ;
                else ev = { .ev = EV_BTN_PRESSED, .d1 = c-8 , .d2 = r } ;
                UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
            }
            else {
              if ( c >= 8 ) {
                if (  BtnScanStates[r][c] < BT_HOLD_THRESHOLD ) BtnScanStates[r][c]++;
                else if (  BtnScanStates[r][c] == BT_HOLD_THRESHOLD ) {
                  BtnScanStates[r][c] ++;
                  UserEvent_t ev = { .ev = EV_BTN_HOLDED, .d1 = c-8 , .d2 = r } ;
                  UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
                }
              }
            } // else
          }
          else if ( BtnScanStates[r][c] ) {
                  BtnScanStates[r][c] = 0;
                  UserEvent_t ev;
                  if ( c < 8 ) ev = { .ev = EV_PAD_RELEASED, .d1 = c, .d2 = r } ;
                  else ev = { .ev = EV_BTN_RELEASED, .d1 = c-8 , .d2 = r } ;
                  UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
          }
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],1);
    }
//  }
    if (++c == sizeof(ScanButtonsColumns) ) c =0;
}

///////////////////////////////////////////////////////////////////////////////
// Set a pad color (0-63)
///////////////////////////////////////////////////////////////////////////////
void PadSetColor(uint8_t padIdx,uint8_t color) {
  if (padIdx >= PAD_SIZE) return;
  if (color == PadColorsCurrent[padIdx]) return;

  PadColorsCurrent[padIdx] = color  ;
  RGBMaskUpdate(padIdx);
}

///////////////////////////////////////////////////////////////////////////////
// Temporary save the full pad current color set to a temporary set
///////////////////////////////////////////////////////////////////////////////
void PadColorsSave() {
  memcpy(PadColorsBackup,PadColorsCurrent,PAD_SIZE);  ;
}

///////////////////////////////////////////////////////////////////////////////
// Restore a temporary saved full pad color set in the current color set
///////////////////////////////////////////////////////////////////////////////
void PadColorsRestore(uint8_t padIdx) {
  memcpy(PadColorsCurrent,PadColorsBackup,PAD_SIZE);
  RGBMaskUpdateAll();
}

///////////////////////////////////////////////////////////////////////////////
// Set the pads background
///////////////////////////////////////////////////////////////////////////////
void PadColorsBackground(uint8_t color) {
  memset(PadColorsCurrent,color,PAD_SIZE);
  RGBMaskUpdateAll();
}

///////////////////////////////////////////////////////////////////////////////
// Set pads color row
//-----------------------------------------------------------------------------
// Mode 0 COLOR_LINE  : Draw a column c at padIdx
// Mode 1 COLOR_COL   : Draw a line   l at padIdx
// Mode 3 COLOR_CROSS : Draw a cross at padIdx
///////////////////////////////////////////////////////////////////////////////
void PadColorsRow(uint8_t mode, uint8_t padIdx,uint8_t color) {
  if (padIdx >= PAD_SIZE) return;
  uint8_t l = padIdx / 8;
  uint8_t c = padIdx - 8*l;

  for (uint8_t i=0 ; i != 8 ; i++ ) {
      if (mode == COLOR_LINE || mode == COLOR_CROSS) PadSetColor(l*8+i,color);
      if (mode >= COLOR_COL  || mode == COLOR_CROSS) PadSetColor(c+8*i,color);
  }
}

///////////////////////////////////////////////////////////////////////////////
// Set a PAD led  ON/OFF
///////////////////////////////////////////////////////////////////////////////
void PadSetLed(uint8_t padIdx,uint8_t state) {

  if (padIdx >= PAD_SIZE) return;

  uint8_t i,idx;
  if ( padIdx < 32 ) { i = 0 ;   idx = 31 - padIdx; }
  else { i = 1; idx = 63 - padIdx; }

  if (state == ON) PadLedStates[i] |=  1 << idx ;
  else PadLedStates[i] &= ~( 1<< idx) ;

}

///////////////////////////////////////////////////////////////////////////////
// Set a button LED ON/OFF
//-----------------------------------------------------------------------------
// Bank 0 is 16 bits. First is Set (bit 16), Last is volume (bit 31)
// Bank 1 is 8 bits.  First is 8. Bit 24.
///////////////////////////////////////////////////////////////////////////////
void ButtonSetLed(uint8_t bt,uint8_t state) {
  if (bt >= BT_NB_MAX ) return;

  if (state == ON)
    ButtonsLedStates[ButtonLedBankMap[bt]] |= ButtonLedBankMsk[bt];
  else ButtonsLedStates[ButtonLedBankMap[bt]] &= ~ButtonLedBankMsk[bt];
}

///////////////////////////////////////////////////////////////////////////////
// Get the led state ON/OFF of a button
///////////////////////////////////////////////////////////////////////////////
uint8_t ButtonGetLed(uint8_t bt) {
  if (bt >= BT_NB_MAX ) return OFF;

  return (ButtonsLedStates[ButtonLedBankMap[bt]] & ButtonLedBankMsk[bt] ? ON:OFF);

}

///////////////////////////////////////////////////////////////////////////////
// Set a buttons bar mask
///////////////////////////////////////////////////////////////////////////////
void ButtonsBarSetLedMsk(uint8_t btBar,uint32_t bitsMsk) {

  if (btBar == BT_BAR_MASTER ) {
    ButtonsLedStates[1] = bitsMsk ;
  }
  else if ( btBar == BT_BAR_MODES) {
    ButtonsLedStates[0] &= ( 0x00FFFFFF | bitsMsk) ;
  }
  else if ( btBar == BT_BAR_CONTROLS) {
    ButtonsLedStates[0] &= 0xFF00FFFF | bitsMsk;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Get a buttons bar mask
///////////////////////////////////////////////////////////////////////////////
uint32_t ButtonsBarGetLedMsk(uint8_t btBar) {

  if (btBar == BT_BAR_MASTER ) {
    return ButtonsLedStates[1] ;
  }
  else if ( btBar == BT_BAR_MODES || btBar == BT_BAR_CONTROLS) {
    return ButtonsLedStates[0] ;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Get the current pressed state of a button (not a pad !)
///////////////////////////////////////////////////////////////////////////////
boolean ButtonIsPressed(uint8_t bt) {
  if (bt >= BT_NB_MAX ) return false;
  // r and c are inversed in the scan array
  uint8_t r = bt/8 ;
  uint8_t c = bt - 8*r ;
  return ( BtnScanStates[c][r+8] ? true:false );
}

///////////////////////////////////////////////////////////////////////////////
// Get the current holded state of a button (not a pad !)
///////////////////////////////////////////////////////////////////////////////
boolean ButtonIsHolded(uint8_t bt) {
  if (bt >= BT_NB_MAX ) return false;
  // r and c are inversed in the scan array
  uint8_t r = bt/8 ;
  uint8_t c = bt - 8*r ;
  return ( BtnScanStates[c][r+8] > BT_HOLD_THRESHOLD ? true:false );
}

///////////////////////////////////////////////////////////////////////////////
// Get the current pressed state of a pad (not a button !)
///////////////////////////////////////////////////////////////////////////////
boolean PadIsPressed(uint8_t padIdx) {
  if (padIdx >= PAD_SIZE ) return false;
  // r and c are inversed in the scan array
  uint8_t r = padIdx/8 ;
  uint8_t c = padIdx - 8*r ;
  return ( BtnScanStates[c][r] ? true:false );
}

///////////////////////////////////////////////////////////////////////////////
// Set magic bootloader mode
///////////////////////////////////////////////////////////////////////////////
void BootLoaderMode()
{
   // Write the Magic word bootloader

   RCC_BASE->APB1ENR |=  (RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN) ;
   // Enable write access to the backup registers and the RTC
   PWR_BASE->CR |= PWR_CR_DBP;

   // write register
   BKP_BASE->BOOT_BTL_REGISTER = BOOT_BTL_MAGIC;

   // Disable write
   PWR_BASE->CR &= ~PWR_CR_DBP;
   RCC_BASE->APB1ENR &=  ~(RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN) ;

   // Reset
   nvic_sys_reset();
}

///////////////////////////////////////////////////////////////////////////////
// Process users events (pads & buttons + encoders)
//-----------------------------------------------------------------------------
// Events are stored in a queue (a ring buffer in fact)
// Event will contain the eventId, row & column
///////////////////////////////////////////////////////////////////////////////
static void ProcessUserEvent(UserEvent_t *ev){

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Button pressed and not released
    case EV_BTN_PRESSED:
        // Reset
        if ( idx == BT_SET  && ButtonIsPressed(BT_CONTROL4) ) {
            // RESET = HOLD BT_CONTROL4 & MASTER8 THEN PRESS SET
            if (  ButtonIsPressed(BT_MS8) ) {
              // All pads off
              PadLedStates[0] = PadLedStates[1] = ButtonsLedStates[0] = ButtonsLedStates[1] = 0;
              delay(100);
              nvic_sys_reset();
            }
            else

            // UPDATE = HOLD MODE2 & MASTER7 THEN PRESS SET
            if (  ButtonIsPressed(BT_MS7) ) {
              PadLedStates[0] = PadLedStates[1] = ButtonsLedStates[0] = ButtonsLedStates[1] = 0;
              delay(100);
              BootLoaderMode(); // Do a reset
            }
        }
      break;
    }

    KikpadMod_ProcessUserEvent(ev);

}

///////////////////////////////////////////////////////////////////////////////
// MIDI USB Loop Process
///////////////////////////////////////////////////////////////////////////////
static void USBMidi_Process()
{

  // Retrieve from unconnected state or first call
	if ( MidiUSB.isConnected() ) {

		// Do we have a MIDI USB packet available ?
		if ( MidiUSB.available() ) {
				midiPacket_t pk ;
				pk.i = MidiUSB.readPacket();
        KikpadMod_USBMidiParse(&pk);
		}
	}

}
///////////////////////////////////////////////////////////////////////////////
// SETUP
///////////////////////////////////////////////////////////////////////////////
void setup() {

  Serial.end();

  // Disable JTAG to free pins
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);

  // USB DISC PIN
  pinMode(PA8, OUTPUT);
  FAST_DIGITAL_WRITE(PA8,1);


  // Leds
  pinMode(DMC_EN,OUTPUT_OPEN_DRAIN);
  FAST_DIGITAL_WRITE(PA2,1);
  pinMode(LS_A0,OUTPUT);
  pinMode(LS_A1,OUTPUT);
  pinMode(LS_A2,OUTPUT);

  pinMode(LS_EN,OUTPUT_OPEN_DRAIN);
  FAST_DIGITAL_WRITE(LS_EN,1);
  pinMode(DMC_DAI,OUTPUT);
  pinMode(DMC_DCK,OUTPUT);
  pinMode(DMC_LAT,OUTPUT);

  // BUTTONS

  for (uint8_t i = 0; i< sizeof(ScanButtonsColumns) ; i++ ) {
    pinMode(ScanButtonsColumns[i],OUTPUT_OPEN_DRAIN);
    FAST_DIGITAL_WRITE(ScanButtonsColumns[i],1);
  }

  for (uint8_t i = 0; i< sizeof(ScanButtonsRows) ; i++ ) {
    pinMode(ScanButtonsRows[i],INPUT_PULLUP );
    FAST_DIGITAL_WRITE(ScanButtonsRows[i],1);
  }

  // Encoders
  pinMode(EC_LS_A0,OUTPUT);
  pinMode(EC_LS_A1,OUTPUT);
  pinMode(EC_LS_A2,OUTPUT);

  for (uint8_t i=0; i!=8 ; i++ ) SPRotary[i].begin(EC_KA,EC_KB);

  // Timer 2 for RGB pad colors,
  // Timer 3 for buttons , and encoders

  RGBRefreshTim2.pause();
  UserEventsTim3.pause();

  RGBRefreshTim2.setPeriod(TIMER_RGB_PERIOD); // in microseconds
  RGBRefreshTim2.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  RGBRefreshTim2.setCompare(TIMER_CH1, 1);
  RGBRefreshTim2.attachInterrupt(TIMER_CH1, RGBTim2Handler);

  UserEventsTim3.setPeriod(TIMER_USER_EVENTS_PERIOD); // in microseconds
  UserEventsTim3.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  UserEventsTim3.setCompare(TIMER_CH1, 1);
  UserEventsTim3.attachInterrupt(TIMER_CH1, UserEventsTim3Handler);

  RGBRefreshTim2.refresh();
  UserEventsTim3.refresh();

  // Reset all leds
  WriteDMC(0);
  LedBankSet(LED_BANK_BT2);
  FAST_DIGITAL_WRITE(DMC_EN,0);
  FAST_DIGITAL_WRITE(LS_EN,0);

  // Start timer
  UserEventsTim3.resume();
  RGBRefreshTim2.resume();

  // All Leds On
  PadLedStates[0] = PadLedStates[1] = ButtonsLedStates[0] = ButtonsLedStates[1] = 0xFFFFFFFF;

  // Show all colors
  for (uint8_t i=0; i != 64 ; i ++ ) PadColorsCurrent[i]=i;
  RGBMaskUpdateAll();

  // Start USB Midi
  MidiUSB.begin() ;

  // Retrieve from unconnected state or first call
	while (! MidiUSB.isConnected() ) delay(500);

  // All buttons Leds Off
  ButtonsLedStates[0] = ButtonsLedStates[1] = 0;

  // Show our logo
  memcpy(PadColorsCurrent,KikGenLogo,sizeof(PadColorsCurrent));
  RGBMaskUpdateAll();
  delay(1000);

  // Initialize module
  KikpadMod_Setup();

}
///////////////////////////////////////////////////////////////////////////////
// LOOP
///////////////////////////////////////////////////////////////////////////////
void loop() {
  USBMidi_Process();

  if (UserEventQueue.available() >= sizeof(UserEvent_t) ) {
    UserEvent_t ev;
    UserEventQueue.readBytes((uint8_t *)&ev,sizeof(UserEvent_t));
    ProcessUserEvent(&ev);
  }

  KikpadMod_Loop();

}
