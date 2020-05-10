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

// Timer refresh time for colors, scan lines, and encoders
#define TIMER_RATE_MICROS 50
#define DELAY_DMC 1
#define LED_BANK_SIZE 32
#define LED_BANK_MAX 8
#define PAD_SIZE 64
#define RB_UEVENT_SIZE  32*sizeof(UserEvent_t)
#define ENCODER_PPR 20
//24
//18
//12

// Some leds ON/Off patterns
#define LED_BK_PATTERN1 0B11111111111111111111111111111111
#define LED_BK_PATTERN2 0B01010101010101010101010101010101
#define LED_BK_PATTERN3 0B10101010101010101010101010101010
#define LED_BK_PATTERN4 0B11110000111100001111000011110000
#define LED_BK_PATTERN5 0B00001111000011110000111100001111
#define LED_BK_PATTERN6 0B11111111000000001111111100000000
#define LED_BK_PATTERN7 0B10000001010000100010010000011000

// Led on / off values
enum {
  LED_OFF,
  LED_ON
} LedSwitch;

// Pins of DMC13 and LS138 driving leds
enum {
  DMC_DAI = PB15,
  DMC_DCK = PB14,
  DMC_LAT = PB13,
  DMC_EN  = PB12,
  LS_EN   = PA2,
  LS_A0   = PA3,
  LS_A1   = PA4,
  LS_A2   = PA5,
} LedDriverPins;

// LS138 lines for 8 encoders
enum {
  EC_LS_A0 = PC0,
  EC_LS_A1 = PC1,
  EC_LS_A2 = PC2,
  EC_KA    = PA1,
  EC_KB    = PA0
};

// LS138 address to drive leds for pads H,L, and buttons bars
enum {
  LED_BANK_HPAD_B,
  LED_BANK_HPAD_R,
  LED_BANK_HPAD_G,
  LED_BANK_LPAD_B,
  LED_BANK_LPAD_R,
  LED_BANK_LPAD_G,
  LED_BANK_BT1,
  LED_BANK_BT2,
} LedBankIds;

// 8 Colors RGB enum
typedef enum {
  BLACK   = 0B000,
  RED     = 0B100,
  GREEN   = 0B010,
  BLUE    = 0B001,
  YELLOW  = 0B110,
  MAGENTA = 0B101,
  CYAN    = 0B011,
  WHITE   = 0B111
} ledColor_t;

// Buttons masks in hte appropirate led bank
enum {
  BTMSK_VOLUME   = 0B1,
  BTMSK_SENDA    = 0B10,
  BTMSK_SENDB    = 0B100,
  BTMSK_PAN      = 0B1000,
  BTMSK_CONTROL1 = 0B10000,
  BTMSK_CONTROL2 = 0B100000,
  BTMSK_CONTROL3 = 0B1000000,
  BTMSK_CONTROL4 = 0B10000000,
  BTMSK_UP       = 0B100000000,
  BTMSK_DOWN     = 0B1000000000,
  BTMSK_LEFT     = 0B10000000000,
  BTMSK_RIGHT    = 0B100000000000,
  BTMSK_CLIP     = 0B1000000000000,
  BTMSK_MODE1    = 0B10000000000000,
  BTMSK_MODE2    = 0B100000000000000,
  BTMSK_SET      = 0B1000000000000000,
  BTMSK_MS1      = 1,
  BTMSK_MS2      = 10,
  BTMSK_MS3      = 100,
  BTMSK_MS4      = 1000,
  BTMSK_MS5      = 10000,
  BTMSK_MS6      = 100000,
  BTMSK_MS7      = 1000000,
  BTMSK_MS8      = 10000000,
} btnmask;


// Events names for buttons (pads are managed differently)
enum {
  BT_MS1,   BT_MS2,  BT_MS3,  BT_MS4,   BT_MS5,  BT_MS6,     BT_MS7,     BT_MS8,
  BT_UP,    BT_DOWN, BT_LEFT, BT_RIGHT, BT_CLIP, BT_MODE1,   BT_MODE2,   BT_SET,
  E_VOLUME, BT_SENDA,BT_SENDB,BT_PAN,BT_CONTROL1,BT_CONTROL2,BT_CONTROL3,BT_CONTROL4,
} ButtonEventTypes;

// User Event type
typedef enum {
EV_NONE,
EV_BTN_PRESSED,
EV_BTN_RELEASED,
EV_EC_CW,
EV_EC_CCW,
} UserEventType_t;

typedef struct{
  uint8_t event;
  uint8_t info1;
  uint8_t info2;
}  __packed UserEvent_t;
