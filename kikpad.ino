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
#include "kikpad.h"
#include "ringbuffer.h"
#include "rotary.h"

// TIMER
HardwareTimer UserEventsTim3(3);

// Queue (ring buffer) for User events
RingBuffer<uint8_t,RB_UEVENT_SIZE> UserEventQueue;

// Buttons Pad & bars scan lines
const uint8_t    ButtonsRows[]      =  {PC8,PC9,PC10,PC11,PC12,PC13,PC14,PC15};
const uint8_t    ButtonsColumns[]   =  {PB0,PB1,PB2,PB3,PB4,PB5,PB6,PB7,PB8,PB9,PB10};

// Encoders
Rotary SPRotary[8];

// 8 colors palette
const ledColor_t LedColorsPalette[] =  {BLACK,RED, GREEN, BLUE,YELLOW, MAGENTA,CYAN,WHITE};
// Led bank color map, in the same order than enum LedBankIds
const ledColor_t PadLedBanksColorMap[] = { BLUE,RED,GREEN,BLUE,RED,GREEN,};

// Current led states (ON/Off)
// Pads : Higer / Lower
// Buttons : Left, bottom / Right
volatile uint32_t  PadLedStates[]        = { LED_BK_PATTERN1, LED_BK_PATTERN1 };
volatile uint32_t  ButtonsLedStates[]    = { LED_BK_PATTERN1, LED_BK_PATTERN1 };

// Current pad colors set
uint8_t PadColors[] {
   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN,

   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN
};

// //const uint8_t PadColorsPattern[][] = {
//
//   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
//   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
//   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
//   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN,
//   BLACK, RED,   GREEN,  BLUE,    YELLOW,  MAGENTA, CYAN,  WHITE,
//   RED,   GREEN, BLUE,   YELLOW,  MAGENTA, CYAN,    WHITE, BLACK,
//   GREEN, BLUE,  YELLOW, MAGENTA, CYAN,    WHITE,   BLACK, RED,
//   BLUE,  YELLOW,MAGENTA,CYAN,    WHITE,   BLACK,   RED,   GREEN
//
// }


// Temporary pad color save
uint8_t PadColorsSave[PAD_SIZE];

// buttons scan lines states (todo replace with binary)
volatile int ButtonsStates[8][11] = {
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
          {0,0,0,0,0,0,0,0,0,0,0},
                           };

volatile uint8_t ScanCols[11] = { 0,0,0,0,0,0,0,0,0,0,0 };




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
  digitalWrite(DMC_DCK,LOW);
  digitalWrite(DMC_LAT,LOW);
  delayMicroseconds(DELAY_DMC);
  for ( uint8_t i = 0; i!= 32 ; i++ ) {
    if ( mask & 1 ) digitalWrite(DMC_DAI, HIGH );
    else digitalWrite(DMC_DAI, LOW );
    delayMicroseconds(DELAY_DMC);
    digitalWrite(DMC_DCK,HIGH);
    delayMicroseconds(DELAY_DMC);
    digitalWrite(DMC_DCK,LOW);
    delayMicroseconds(DELAY_DMC);
    mask >>= 1;
  }
  digitalWrite(DMC_LAT,HIGH);
  delayMicroseconds(DELAY_DMC);
  digitalWrite(DMC_LAT,LOW);
  delayMicroseconds(DELAY_DMC);
}

boolean SetLedBank(uint8_t addr ) {
  static uint8_t lastAddr = 0xFF;
  if ( addr == lastAddr  ) return false;

  lastAddr = addr;

  if ( addr & 1 ) digitalWrite(LS_A0, HIGH  ) ;
  else digitalWrite(LS_A0, LOW  ) ;

  if ( addr & 2 ) digitalWrite(LS_A1, HIGH  ) ;
  else digitalWrite(LS_A1, LOW  ) ;

  if ( addr & 4 ) digitalWrite(LS_A2, HIGH  ) ;
  else digitalWrite(LS_A2, LOW  ) ;

  return true;
}

boolean SetEncoderTo(uint8_t addr ) {
  static uint8_t lastAddr = 0xFF;
  if ( addr == lastAddr  ) return false;

  lastAddr = addr;

  if ( addr & 1 ) digitalWrite(EC_LS_A0, HIGH  ) ;
  else digitalWrite(EC_LS_A0, LOW  ) ;

  if ( addr & 2 ) digitalWrite(EC_LS_A1, HIGH  ) ;
  else digitalWrite(EC_LS_A1, LOW  ) ;

  if ( addr & 4 ) digitalWrite(EC_LS_A2, HIGH  ) ;
  else digitalWrite(EC_LS_A2, LOW  ) ;

  return true;
}

// 374 uS
//  ISR
void RGBTim3Handler() {

  static uint8_t ledPadBk = 0;
  static uint8_t ledBtBk = LED_BANK_BT1;

  // LEDs /////////////////////////////////////////////////////////////////////
  // Compose RGB colors

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
      if ( ( PadColors[ofs - i] & PadLedBanksColorMap[ledPadBk] ) && (PadLedStates[idx] & (1<<i) ) ) dmcMask |= 1<<i;
    }

    if ( dmcMask ) {
      SetLedBank(ledPadBk);
      WriteDMC(dmcMask);
    }

    ledPadBk++;
  }
  // Alternate Buttons led bank to equilibrate timings between colors
  else {
    ledPadBk = 0;
    SetLedBank(ledBtBk);
    WriteDMC(ButtonsLedStates[ledBtBk-6]);
    if ( ++ledBtBk == 8 )  ledBtBk = LED_BANK_BT1;
  }

}

// 13 us without event
void UserEventsTim3Handler() {
  static uint8_t col = 0;
  // Buttons //////////////////////////////////////////////////////////////////
  // 8 rows, 11 columns
  digitalWrite(ButtonsColumns[col],LOW);
  for ( uint8_t r = 0 ; r != sizeof(ButtonsRows) ; r++ ) {
        if ( digitalRead(ButtonsRows[r]) == LOW) {
            //if ( (ScanCols[r] & (1 << col)) == 0 ) {
              //    ScanCols[r] |= (1 << col);
        if (ButtonsStates[r][col] == 0 ) {
                  ButtonsStates[r][col] = 1;
                  UserEvent_t ev = { .event = EV_BTN_PRESSED, .info1 = r, .info2 = col } ;
                  UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
            }
        }
//        else if ( (ScanCols[r] & (1 << col)) == 1 ) {
//                ScanCols[r] &= ~(1 << col);
        else if ( ButtonsStates[r][col] == 1 ) {
                ButtonsStates[r][col] = 0;
                UserEvent_t ev = { .event = EV_BTN_RELEASED, .info1 = r, .info2 = col } ;
                UserEventQueue.write((uint8_t*)&ev,sizeof(UserEvent_t));
        }
  }
  digitalWrite(ButtonsColumns[col],HIGH);
  if ( ++col == 11 )  col = 0;
}

void SetPadColor(uint8_t padIdx,uint8_t color) {
  if (padIdx > 63) return;
  PadColors[padIdx] = color  ;
}

void PadColorSave(uint8_t padIdx) {
  if (padIdx > 63) return;
  PadColorsSave[padIdx] = PadColors[padIdx]  ;
}

void PadColorRestore(uint8_t padIdx) {
  if (padIdx > 63) return;
  PadColors[padIdx] = PadColorsSave[padIdx] ;
}

void SetPadColorBackground(uint8_t color) {
  memset(PadColors,color,sizeof(PadColors));
}

void SetPad(uint8_t padIdx,uint8_t state) {

  if (padIdx > 63) return;

  uint8_t i,of;

  if ( padIdx < 32 ) {
    i = 0 ;
    of = 31;
  }
  else {
    i = 1;
    of = 63;
  }

  if (state) PadLedStates[i] |=  1 << (of - padIdx) ;
  else PadLedStates[i] &= ~( 1<< (of - padIdx) );
}



void ProcessUserEvent(UserEvent_t *ev){

  uint8_t idx = ev->info1*8 + ev->info2;

  static uint8_t pad=0;

  if (ev->event == EV_EC_CW ) {
    for (uint8_t i ; i < 8 ; i++ ) {
        SetPadColor(ev->info1+8*i,RED);
    }

    return;
  }

  if (ev->event == EV_EC_CCW ) {
    for (uint8_t i ; i < 8 ; i++ ) {
        SetPadColor(ev->info1+8*i,BLUE);  
    }
    return;
  }


  // Pad
  if (ev->info1 < 8 && ev->info2 < 8 ) {
    uint8_t idx = ev->info1 + 8*ev->info2;
    if (ev->event == EV_BTN_RELEASED ) {
      SetPadColor(idx,  ev->info1);
    }
  }
  else {
    // Button
    uint8_t idx = ev->info1*8 + ev->info2 -8 ;


  }

  SerialPrintf("Event %d %d %d%n",ev->event,ev->info1,ev->info2);
//Serial.println("Event !");
}

// 12us without EV
void EncodersSwitchingTim3Handler() {
  static uint8_t ecAddr = 0;

  if ( ecAddr & 1 ) digitalWrite(EC_LS_A0, HIGH  ) ;
  else digitalWrite(EC_LS_A0, LOW  ) ;

  if ( ecAddr & 2 ) digitalWrite(EC_LS_A1, HIGH  ) ;
  else digitalWrite(EC_LS_A1, LOW  ) ;

  if ( ecAddr & 4 ) digitalWrite(EC_LS_A2, HIGH  ) ;
  else digitalWrite(EC_LS_A2, LOW  ) ;

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


void setup() {

  // Disable JTAG to free pins
  disableDebugPorts();

  // USB DISC PIN
  gpio_set_mode(PIN_MAP[PA8].gpio_device, PIN_MAP[PA8].gpio_bit, GPIO_OUTPUT_PP);
  gpio_write_bit(PIN_MAP[PA8].gpio_device, PIN_MAP[PA8].gpio_bit, 1);
  delay(2000);

  Serial.begin(115200);
  delay(4000);

  // Leds
  pinMode(DMC_EN,OUTPUT_OPEN_DRAIN);
  digitalWrite(PA2,HIGH);
  pinMode(LS_A0,OUTPUT);
  pinMode(LS_A1,OUTPUT);
  pinMode(LS_A2,OUTPUT);

  pinMode(LS_EN,OUTPUT_OPEN_DRAIN);
  digitalWrite(LS_EN,HIGH);
  pinMode(DMC_DAI,OUTPUT);
  pinMode(DMC_DCK,OUTPUT);
  pinMode(DMC_LAT,OUTPUT);

  // BUTTONS

  for (uint8_t i = 0; i< sizeof(ButtonsColumns) ; i++ ) {
    pinMode(ButtonsColumns[i],OUTPUT_OPEN_DRAIN);
    digitalWrite(ButtonsColumns[i],HIGH);
  }

  for (uint8_t i = 0; i< sizeof(ButtonsRows) ; i++ ) {
    pinMode(ButtonsRows[i],INPUT);
  }

  // Encoders
  pinMode(EC_LS_A0,OUTPUT);
  pinMode(EC_LS_A1,OUTPUT);
  pinMode(EC_LS_A2,OUTPUT);

  for (uint8_t i=0; i!=8 ; i++ ) SPRotary[i].begin(EC_KA,EC_KB);

  // Timer 3 for buttons , RGB pad colors, and encoders
  UserEventsTim3.pause();
  UserEventsTim3.setPeriod(TIMER_RATE_MICROS); // in microseconds
  UserEventsTim3.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  UserEventsTim3.setCompare(TIMER_CH1, 8); // 400us
  UserEventsTim3.attachInterrupt(TIMER_CH1, RGBTim3Handler);

  UserEventsTim3.setMode(TIMER_CH2, TIMER_OUTPUT_COMPARE);
  UserEventsTim3.setCompare(TIMER_CH2, 4); // 200 us
  UserEventsTim3.attachInterrupt(TIMER_CH2, UserEventsTim3Handler);

  UserEventsTim3.setMode(TIMER_CH3, TIMER_OUTPUT_COMPARE);
  UserEventsTim3.setCompare(TIMER_CH3, 1); // 1 = 50us
  UserEventsTim3.attachInterrupt(TIMER_CH3, EncodersSwitchingTim3Handler);
  UserEventsTim3.refresh();

  // Reset all leds
  WriteDMC(0);
  SetLedBank(LED_BANK_BT2);
  digitalWrite(DMC_EN,LOW);
  digitalWrite(LS_EN,LOW);

  // Start timer
  UserEventsTim3.resume();
}

void loop() {


//
//   for ( uint8_t i = 0 ; i != sizeof(ButtonsColumns) ; i++ ) {
//       digitalWrite(ButtonsColumns[i],HIGH);
//       delayMicroseconds(10);
//   }
//
//
// while (1) {
//
//
//   for (uint8_t col = 0; col != sizeof(ButtonsColumns) ; col++ ) {
//
//     // Buttons //////////////////////////////////////////////////////////////////
//     // 8 rows, 11 columns
//     digitalWrite(ButtonsColumns[col],LOW);
//
//     for ( uint8_t r = 0 ; r != sizeof(ButtonsRows) ; r++ ) {
//           if ( digitalRead(ButtonsRows[r]) == LOW)
//             SetPadColor(r,RED);
//           else SetPadColor(r,WHITE);
//     }
//
//     digitalWrite(ButtonsColumns[col],HIGH);
//   }
// }

// unsigned long t1 = micros();
// EncodersSwitchingTim3Handler() ;
// t1 = micros()- t1;
// SerialPrintf("EncodersSwitchingTim3Handler() time %d%n",t1);
// delay(100);
// t1 = micros();
// UserEventsTim3Handler() ;
// t1 = micros()- t1;
// SerialPrintf("UserEventsTim3Handler() time %d%n",t1);
// delay(100);
// t1 = micros();
// RGBTim3Handler() ;
// t1 = micros()- t1;
// SerialPrintf("RGBTim3Handler() time %d%n",t1);
// delay(1000);



  if (UserEventQueue.available() >= sizeof(UserEvent_t) ) {
    UserEvent_t ev;
    UserEventQueue.readBytes((uint8_t *)&ev,sizeof(UserEvent_t));
    ProcessUserEvent(&ev);
  }


}
