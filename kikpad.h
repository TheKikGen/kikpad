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


#ifndef _KIKPAD_H_
#define _KIKPAD_H_

// Timer refresh time for colors, scan lines, and encoders
#define TIMER_RGB_PERIOD 300
#define TIMER_USER_EVENTS_PERIOD 50
#define DELAY_DMC 1
#define LED_BANK_SIZE 32
#define LED_BANK_MAX 8
#define PAD_COLOR_DEPTH 3
#define PAD_SIZE 64
#define RB_UEVENT_SIZE  32*sizeof(UserEvent_t)

// Sysex internal buffer size
#define GLOBAL_DATA_BUFF_SIZE 64

// LED ON recovery time in msec when no dedicated LED for CONNECT USB
#define LED_CONNECT_USB_RECOVER_TIME_MILLIS 500

// Boot modes magic words
#define BOOT_BTL_MAGIC        0x424C
#define BOOT_BTL_REGISTER     DR10
#define BOOT_CONFIG_MAGIC     0x0000

// Some leds ON/Off patterns
#define LED_BK_PATTERN1 0B11111111111111111111111111111111
#define LED_BK_PATTERN2 0B01010101010101010101010101010101
#define LED_BK_PATTERN3 0B10101010101010101010101010101010
#define LED_BK_PATTERN4 0B11110000111100001111000011110000
#define LED_BK_PATTERN5 0B00001111000011110000111100001111
#define LED_BK_PATTERN6 0B11111111000000001111111100000000
#define LED_BK_PATTERN7 0B10000001010000100010010000011000

// GPIO fast macros
#define FAST_DIGITAL_WRITE(pin,value) gpio_write_bit(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit, value)
#define FAST_DIGITAL_READ(pin) gpio_read_bit(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit)
#define FAST_PINMODE(pin,mode) gpio_set_mode(PIN_MAP[pin].gpio_device, PIN_MAP[pin].gpio_bit, (gpio_pin_mode) mode)

// Led on / off values
enum {
  OFF,
  ON
} Switch;

// Button hold threshold around 2s.
#define BT_HOLD_THRESHOLD 500

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
  BLACK    = 0B000000,
  BLUE     = 0B000001,
  GREEN    = 0B000010,
  CYAN     = 0B000011,
  RED      = 0B000100,
  MAGENTA  = 0B000101,
  YELLOW   = 0B000110,
  WHITE    = 0B000111,
  ORANGE   = 52,
  LT_RED   = 32,
  LT_GREEN = 50,
} ledColor_t;

enum {
  COLOR_LINE,
  COLOR_COL,
  COLOR_CROSS
} colorLineModEnum;

// Macros to get a button or a pad sequential index in the 8x11 matrix
// 0,0 is the upper left corner
#define SCAN_IDX(r,c) ( 8 * r + c )

// Buttons masks in the appropirate led bank
enum {
  BTMSK_VOLUME   = 0B00000000100000000000000000000000,
  BTMSK_SENDA    = 0B00000000010000000000000000000000,
  BTMSK_SENDB    = 0B00000000001000000000000000000000,
  BTMSK_PAN      = 0B00000000000100000000000000000000,
  BTMSK_CONTROL1 = 0B00000000000010000000000000000000,
  BTMSK_CONTROL2 = 0B00000000000001000000000000000000,
  BTMSK_CONTROL3 = 0B00000000000000100000000000000000,
  BTMSK_CONTROL4 = 0B00000000000000010000000000000000,

  BTMSK_UP       = 0B10000000000000000000000000000000,
  BTMSK_DOWN     = 0B01000000000000000000000000000000,
  BTMSK_LEFT     = 0B00100000000000000000000000000000,
  BTMSK_RIGHT    = 0B00010000000000000000000000000000,
  BTMSK_CLIP     = 0B00001000000000000000000000000000,
  BTMSK_MODE1    = 0B00000100000000000000000000000000,
  BTMSK_MODE2    = 0B00000010000000000000000000000000,
  BTMSK_SET      = 0B00000001000000000000000000000000,

  BTMSK_MS1      = 0B10000000000000000000000000000000,
  BTMSK_MS2      = 0B01000000000000000000000000000000,
  BTMSK_MS3      = 0B00100000000000000000000000000000,
  BTMSK_MS4      = 0B00010000000000000000000000000000,
  BTMSK_MS5      = 0B00001000000000000000000000000000,
  BTMSK_MS6      = 0B00000100000000000000000000000000,
  BTMSK_MS7      = 0B00000010000000000000000000000000,
  BTMSK_MS8      = 0B00000001000000000000000000000000,
} ButtonLedBankMaskEnum;

// Buttons bar names
enum {
BT_BAR_MASTER,
BT_BAR_MODES,
BT_BAR_CONTROLS,
} ButtonsBarNames;

// Events names for buttons (pads are managed differently)
enum {
  BT_MS1,   BT_MS2,  BT_MS3,  BT_MS4,  BT_MS5,     BT_MS6,     BT_MS7,     BT_MS8,
  BT_UP,    BT_DOWN, BT_LEFT, BT_RIGHT,BT_CLIP,    BT_MODE1,   BT_MODE2,   BT_SET,
  BT_VOLUME,BT_SENDA,BT_SENDB,BT_PAN,  BT_CONTROL1,BT_CONTROL2,BT_CONTROL3,BT_CONTROL4,
  BT_NB_MAX // Max buttons number
} ButtonEventNames;

// User Event type
typedef enum {
EV_NONE,
EV_ERROR,
EV_BTN_PRESSED,
EV_BTN_RELEASED,
EV_BTN_HOLDED,
EV_PAD_PRESSED,
EV_PAD_RELEASED,
EV_EC_CW,
EV_EC_CCW,
} UserEventType_t;


// Midi Control change
enum {
  CC_VOL = 7,
  CC_PAN = 10,
  CC_EFFECT1 = 12,
  CC_EFFECT2 = 13,
  CC_CUTOFF = 71,
  CC_RESO = 74,

} MidiControlChangeEnum;

// High level user event management
typedef struct{
  uint8_t ev;
  uint8_t d1;
  uint8_t d2;
}  __packed UserEvent_t;

// Use this structure to send and receive packet to/from USB /serial/BUS
typedef union  {
    uint32_t i;
    uint8_t  packet[4];
} __packed midiPacket_t;


// Functions prototypes
void SerialPrintf(const char *format, ...)  ;
void RGBMaskUpdate(uint8_t padIdx);
void RGBMaskUpdateAll(void);
void PadSetColor(uint8_t padIdx,uint8_t color);
void PadColorsSave(void);
void PadColorsRestore(uint8_t padIdx);
void PadColorsBackground(uint8_t color);
void PadColorsRow(uint8_t mode, uint8_t padIdx,uint8_t color);
void PadSetLed(uint8_t padIdx,uint8_t state);
void ButtonSetLed(uint8_t bt,uint8_t state);
uint8_t ButtonGetLed(uint8_t bt);
void ButtonsBarSetLedMsk(uint8_t btBar,uint32_t bitMsk);
boolean ButtonIsPressed(uint8_t bt);
boolean PadIsPressed(uint8_t padIdx);

#endif
