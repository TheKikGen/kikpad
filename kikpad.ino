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

#include <string.h>
#include <stdarg.h>

#include <libmaple/nvic.h>
#include "libmaple/flash.h"
#include "libmaple/pwr.h"
#include "libmaple/rcc.h"
#include "libmaple/bkp.h"

#include "kikpad.h"
#include "usb_midi.h"
#include "ringbuffer.h"
#include "rotary.h"

// TIMER
HardwareTimer RGBRefreshTim2(2);
HardwareTimer UserEventsTim3(3);

// Queue (ring buffer) for User events
RingBuffer<uint8_t,RB_UEVENT_SIZE> UserEventQueue;

// Buttons Pad & bars scan lines
const uint8_t    ScanButtonsRows[]      =  {PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15};
const uint8_t    ScanButtonsColumns[]   =  {PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10};

// Encoders
Rotary SPRotary[8];

// Led bank color map, in the same order than enum LedBankIds
const ledColor_t PadLedBanksColorMap[] = { BLUE,RED,GREEN,BLUE,RED,GREEN,};

// Buttons Banks and mask settings
const uint8_t ButtonLedBankMsk[] = {
  BTMSK_MS1, BTMSK_MS2, BTMSK_MS3, BTMSK_MS4, BTMSK_MS5,BTMSK_MS6, BTMSK_MS7, BTMSK_MS8,
  BTMSK_UP, BTMSK_DOWN, BTMSK_LEFT,  BTMSK_RIGHT, BTMSK_CLIP, BTMSK_MODE1,  BTMSK_MODE2, BTMSK_SET,
  BTMSK_VOLUME, BTMSK_SENDA,BTMSK_SENDB,BTMSK_PAN,BTMSK_CONTROL1,BTMSK_CONTROL2,BTMSK_CONTROL3,BTMSK_CONTROL4,
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
volatile uint32_t  ButtonsLedStates[]    = { LED_BK_PATTERN5, LED_BK_PATTERN3 };

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


// Temporary pad color save
uint8_t PadColorsBackup[PAD_SIZE];

// buttons scan lines states (todo replace with binary)
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

//volatile uint8_t ScanCols[11] = { 0,0,0,0,0,0,0,0,0,0,0 };

// USB Midi object & globals
USBMidi MidiUSB;


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

void WriteDMC(uint32_t mask) {
  FAST_DIGITAL_WRITE(DMC_DCK,0);
  FAST_DIGITAL_WRITE(DMC_LAT,0);
//  delayMicroseconds(DELAY_DMC);
  for ( uint8_t i = 0; i!= 32 ; i++ ) {
    if ( mask & 1 ) FAST_DIGITAL_WRITE(DMC_DAI, 1 );
    else FAST_DIGITAL_WRITE(DMC_DAI, 0 );
//    delayMicroseconds(DELAY_DMC);
    FAST_DIGITAL_WRITE(DMC_DCK,1);
//    delayMicroseconds(DELAY_DMC);
    FAST_DIGITAL_WRITE(DMC_DCK,0);
//    delayMicroseconds(DELAY_DMC);
    mask >>= 1;
  }
  FAST_DIGITAL_WRITE(DMC_LAT,1);
//  delayMicroseconds(DELAY_DMC);
  FAST_DIGITAL_WRITE(DMC_LAT,0);
//  delayMicroseconds(DELAY_DMC);
}

boolean LedBankSet(uint8_t addr ) {
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

boolean EncoderBankSet(uint8_t addr ) {
  static uint8_t lastAddr = 0xFF;
  if ( addr == lastAddr  ) return false;

  lastAddr = addr;

  if ( addr & 1 ) FAST_DIGITAL_WRITE(EC_LS_A0, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A0, 0  ) ;

  if ( addr & 2 ) FAST_DIGITAL_WRITE(EC_LS_A1, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A1, 0  ) ;

  if ( addr & 4 ) FAST_DIGITAL_WRITE(EC_LS_A2, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A2, 0  ) ;

  return true;
}


// 374 uS
//  ISR
void RGBTim3Handler() {

  static uint8_t ledPadBk = 0;
  static uint8_t ledBtBk = LED_BANK_BT1;
  static uint8_t  colorQ = 0;
  static uint8_t  colorShift = 0;

  // LEDs /////////////////////////////////////////////////////////////////////
  // RGB colors quantization.
  // 64 colors - rgbRGB (EGA)

  // Ligh off last bank.
  WriteDMC(0);

  // Pads
  if ( ledPadBk < LED_BANK_BT1 ) {
    uint32_t dmcMask = 0;
    uint8_t  ofs;
    uint8_t  idx;

    if ( ledPadBk < LED_BANK_LPAD_B ) {
       ofs = 31;
       idx = 0;
    } else {
       ofs = 63;
       idx = 1;
    }

    // Colors from pad 0 to pad 31 / pad 32 to 63. Order is reverse.
    for ( uint8_t i = 0; i != 32 ; i++ ) {
      if ( ( (PadColorsCurrent[ofs - i]>>colorShift) & PadLedBanksColorMap[ledPadBk] )
                 && (PadLedStates[idx] & (1<<i) ) ) dmcMask |= 1<<i;
    }

  //  if ( dmcMask ) {
      LedBankSet(ledPadBk);
      WriteDMC(dmcMask);
//    }

    ledPadBk++;
  }
  // Alternate Buttons led bank to equilibrate timings between colors
  else {
    ledPadBk = 0;
    if ( ++colorQ == 3 ) colorQ = 0;
    colorShift = colorQ < 2 ? 0:3;  // Manage msb/lsb

    LedBankSet(ledBtBk);
    WriteDMC(ButtonsLedStates[ledBtBk-6]);
    if ( ++ledBtBk ==  LED_BANK_MAX )  ledBtBk = LED_BANK_BT1;
  }

}

// User events handler
void UserEventsTim3Handler() {
  static uint8_t ecAddr = 0;

  // Encoders /////////////////////////////////////////////////////////////////

  if ( ecAddr & 1 ) FAST_DIGITAL_WRITE(EC_LS_A0, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A0, 0  ) ;

  if ( ecAddr & 2 ) FAST_DIGITAL_WRITE(EC_LS_A1, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A1, 0  ) ;

  if ( ecAddr & 4 ) FAST_DIGITAL_WRITE(EC_LS_A2, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A2, 0  ) ;

  delayMicroseconds(4);

  uint8_t state = SPRotary[ecAddr].read();

  if (state == Rotary::dirCw ) {
       UserEvent_t ev = { .event = EV_EC_CW, .info1 = ecAddr, .info2 = 0 } ;
       UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));

  } else if (state == Rotary::dirCCw ) {
      UserEvent_t ev = { .event = EV_EC_CCW, .info1 = ecAddr, .info2 = 0 } ;
      UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
  }

  if (++ecAddr == 8) ecAddr = 0;

  // Buttons /////////////////////////////////////////////////////////////////
  static uint8_t c = 0;
    //for ( uint8_t c = 0 ; c!= sizeof(ScanButtonsColumns); c++ ) {
    for ( uint8_t r = 0 ; r !=sizeof(ScanButtonsRows) ;r++  ) {
          FAST_DIGITAL_WRITE(ScanButtonsRows[r],1);
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],0);
          if ( FAST_DIGITAL_READ(ScanButtonsRows[r]) == 0 ) {
            if (BtnScanStates[r][c] == 0 ) {
                BtnScanStates[r][c] = 1;
                UserEvent_t ev;
                if ( c < 8 ) ev = { .event = EV_PAD_PRESSED, .info1 = r, .info2 = c } ;
                else ev = { .event = EV_BTN_PRESSED, .info1 = r + (c-8)*8, .info2 = 0 } ;
                UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
            }
            else {
              BtnScanStates[r][c]++;
              if (  BtnScanStates[r][c] >= BT_HOLD_THRESHOLD ) {
                BtnScanStates[r][c] = 0; // No release event
                UserEvent_t ev;
                if ( c < 8 ) ev = { .event = EV_PAD_HOLDED, .info1 = r, .info2 = c } ;
                else ev = { .event = EV_BTN_HOLDED, .info1 = r + (c-8)*8, .info2 = 0 } ;
              }
            }
          }
          else if ( BtnScanStates[r][c] ) {
                  BtnScanStates[r][c] = 0;
                  UserEvent_t ev;
                  if ( c < 8 ) ev = { .event = EV_PAD_RELEASED, .info1 = r, .info2 = c } ;
                  else ev = { .event = EV_BTN_RELEASED, .info1 = r + (c-8)*8, .info2 = r } ;
                  UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
          }
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],1);
    }
//  }
    if (++c == sizeof(ScanButtonsColumns) ) c =0;
}


// Buttons //////////////////////////////////////////////////////////////////
// 8 rows, 11 columns
// 350 us without event
void ScanButtonsTim3Handler() {
  static uint8_t c = 0;
    //for ( uint8_t c = 0 ; c!= sizeof(ScanButtonsColumns); c++ ) {
    for ( uint8_t r = 0 ; r !=sizeof(ScanButtonsRows) ;r++  ) {
          FAST_DIGITAL_WRITE(ScanButtonsRows[r],1);
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],0);
          if ( FAST_DIGITAL_READ(ScanButtonsRows[r]) == 0 ) {
            if (BtnScanStates[r][c] == 0 ) {
                BtnScanStates[r][c] = 1;
                UserEvent_t ev;
                if ( c < 8 ) ev = { .event = EV_PAD_PRESSED, .info1 = r, .info2 = c } ;
                else ev = { .event = EV_BTN_PRESSED, .info1 = r + (c-8)*8, .info2 = 0 } ;
                UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
            }
            else {
              BtnScanStates[r][c]++;
              if (  BtnScanStates[r][c] >= BT_HOLD_THRESHOLD ) {
                BtnScanStates[r][c] = 0; // No release event
                UserEvent_t ev;
                if ( c < 8 ) ev = { .event = EV_PAD_HOLDED, .info1 = r, .info2 = c } ;
                else ev = { .event = EV_BTN_HOLDED, .info1 = r + (c-8)*8, .info2 = 0 } ;
              }
            }
          }
          else if ( BtnScanStates[r][c] ) {
                  BtnScanStates[r][c] = 0;
                  UserEvent_t ev;
                  if ( c < 8 ) ev = { .event = EV_PAD_RELEASED, .info1 = r, .info2 = c } ;
                  else ev = { .event = EV_BTN_RELEASED, .info1 = r + (c-8)*8, .info2 = r } ;
                  UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
          }
          FAST_DIGITAL_WRITE(ScanButtonsColumns[c],1);
    }
//  }
    if (++c == sizeof(ScanButtonsColumns) ) c =0;
}

// 12us without EV
void EncodersSwitchingTim3Handler() {
  static uint8_t ecAddr = 0;

  if ( ecAddr & 1 ) FAST_DIGITAL_WRITE(EC_LS_A0, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A0, 0  ) ;

  if ( ecAddr & 2 ) FAST_DIGITAL_WRITE(EC_LS_A1, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A1, 0  ) ;

  if ( ecAddr & 4 ) FAST_DIGITAL_WRITE(EC_LS_A2, 1  ) ;
  else FAST_DIGITAL_WRITE(EC_LS_A2, 0  ) ;

  delayMicroseconds(4);

  uint8_t state = SPRotary[ecAddr].read();

  if (state == Rotary::dirCw ) {
       UserEvent_t ev = { .event = EV_EC_CW, .info1 = ecAddr, .info2 = 0 } ;
       UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));

  } else if (state == Rotary::dirCCw ) {
      UserEvent_t ev = { .event = EV_EC_CCW, .info1 = ecAddr, .info2 = 0 } ;
      UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
  }

  if (++ecAddr == 8) ecAddr = 0;

}


void PadSetColor(uint8_t padIdx,uint8_t color) {
  if (padIdx >= PAD_SIZE) return;
  PadColorsCurrent[padIdx] = color  ;
}

void PadColorsSave() {
  memcpy(PadColorsBackup,PadColorsCurrent,PAD_SIZE);  ;
}

void PadColorsRestore(uint8_t padIdx) {
  memcpy(PadColorsCurrent,PadColorsBackup,PAD_SIZE);
}

void PadColorsBackground(uint8_t color) {
  memset(PadColorsCurrent,color,PAD_SIZE);
}

void PadSetLed(uint8_t padIdx,uint8_t state) {

  if (padIdx >= PAD_SIZE) return;

  uint8_t i,of;

  if ( padIdx < 32 ) { i = 0 ;   of = 31; }
  else { i = 1; of = 63; }

  if (state == ON) PadLedStates[i] |=  1 << (of - padIdx) ;
  else PadLedStates[i] &= ~( 1<< (of - padIdx) );
}

void ButtonSetLed(uint8_t bt,uint8_t state) {
  if (bt >= BT_NB_MAX ) return;

  if (state == ON)
    ButtonsLedStates[ButtonLedBankMap[bt]] |= ButtonLedBankMsk[bt];
  else ButtonsLedStates[ButtonLedBankMap[bt]] &= ~ButtonLedBankMsk[bt];
}

uint8_t ButtonGetLed(uint8_t bt) {
  if (bt >= 23 ) return 0;

  return (ButtonsLedStates[ButtonLedBankMap[bt]] & ButtonLedBankMsk[bt] ? ON:OFF);

}


void ProcessUserEvent(UserEvent_t *ev){

  // Encoders Clock wise
  if (ev->event == EV_EC_CW ) {
    for (uint8_t i ; i < 8 ; i++ ) {
        PadSetColor(ev->info1+8*i,RED);
    }
    return;
  }
  else

  // Encoders Counter Clock wise
  if (ev->event == EV_EC_CCW ) {
    for (uint8_t i ; i < 8 ; i++ ) {
        PadSetColor(ev->info1+8*i,BLUE);
    }
    return;
  }
  else

  if (ev->event == EV_PAD_RELEASED ) {
      PadSetColor(ev->info1+8*ev->info2,  ev->info1);
  }
  else

  if (ev->event == EV_BTN_RELEASED ) {
      if ( ev->info1 == BT_MS1 ) nvic_sys_reset();
      else if ( ev->info1 == BT_SET ) PadColorsBackground(YELLOW);
      else if ( ev->info1 == BT_CLIP ) PadColorsBackground(WHITE);
      ButtonSetLed(ev->info1,OFF);
  }

  if (ev->event == EV_PAD_HOLDED ) {
      PadSetLed(ev->info1, OFF);
  }

  if (ev->event == EV_BTN_HOLDED ) {
      if ( ev->info1 == BT_CLIP ) PadColorsBackground(GREEN);
  }
}

void setup() {

  Serial.end();

  // Disable JTAG to free pins
  disableDebugPorts();

  // USB DISC PIN
  pinMode(PA8, OUTPUT);
  FAST_DIGITAL_WRITE(PA8,1);
  delay(2000);

  Serial.begin(115200);
  delay(4000);

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
  RGBRefreshTim2.attachInterrupt(TIMER_CH1, RGBTim3Handler);

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

  // Start USB Midi
  //MidiUSB.begin() ;
  //delay(4000); // Note : Usually around 4 s to fully detect USB Midi on the host

  // Start timer
  RGBRefreshTim2.resume();
  UserEventsTim3.resume();

  for (uint8_t i=0; i != 64 ; i ++ ) PadColorsCurrent[i]=i;
}

void loop() {

  if (UserEventQueue.available() >= sizeof(UserEvent_t) ) {
    UserEvent_t ev;
    UserEventQueue.readBytes((uint8_t *)&ev,sizeof(UserEvent_t));
    ProcessUserEvent(&ev);
  }

}
