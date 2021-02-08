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

// KIKPAD_MPCForce : Akai Force controller emulation to work with an MPC Live
// need tkgl_anyctrl.so LD_LOAD library

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_

#define ENCODERS_RELATIVE
#define KIPAD_PAD_DEFAULT_VELOCITY 0x7F
#define FORCE_PAD_OFFSET 0x36

enum ForceButtonsCtrlValue {
  FORCE_NAVIGATE   = 0x00,  FORCE_KNOBS      = 0X01,  FORCE_MENU       = 0x02, FORCE_MATRIX     = 0x03,
  FORCE_NOTE       = 0x04,  FORCE_MASTER     = 0x05,  FORCE_CLIP       = 0x09, FORCE_MIXER      = 0x0B,
  FORCE_LOAD       = 0x23,  FORCE_SAVE       = 0x24,  FORCE_EDIT       = 0x25, FORCE_DELETE     = 0x26,
  FORCE_SHIFT      = 0x31,  FORCE_SELECT     = 0x34,  FORCE_TAP        = 0x35, FORCE_PLUS       = 0x36,
  FORCE_MINUS      = 0x37,  FORCE_LAUNCH_1   = 0x38,  FORCE_LAUNCH_2   = 0x39, FORCE_LAUNCH_3   = 0x3A,
  FORCE_LAUNCH_4   = 0x3B,  FORCE_LAUNCH_5   = 0x3C,  FORCE_LAUNCH_6   = 0x3D, FORCE_LAUNCH_7   = 0x3E,
  FORCE_LAUNCH_8   = 0x3F,  FORCE_UNDO       = 0x43,  FORCE_REC        = 0x49, FORCE_STOP       = 0x51,
  FORCE_PLAY       = 0x52,  FORCE_MUTE       = 0x5B,  FORCE_SOLO       = 0x5C, FORCE_REC_ARM    = 0x5D,
  FORCE_CLIP_STOP  = 0x5E,  FORCE_STOP_ALL   = 0x5F,  FORCE_UP         = 0x70, FORCE_DOWN       = 0x71,
  FORCE_LEFT       = 0x72,  FORCE_RIGHT      = 0x73,  FORCE_LAUNCH     = 0x74, FORCE_STEP_SEQ   = 0x75,
  FORCE_ARP        = 0x76,  FORCE_COPY       = 0x7A,  FORCE_ASSIGN_A   = 0x7B, FORCE_ASSIGN_B   = 0x7C,
};

// Control mapping
typedef struct{
  int8_t ctrl;
  uint8_t type; // 0 : Button. 1 : column pad
}  __packed ForceButtonsMap_t;

// F0 47 7F [3B] 65 00 04 [Pad #] [R] [G] [B] F7
const uint8_t MPCSysexPadColor[] = {0xF0,0x47,0x7F,0xFF,0x65,0x00,0x04,0xFF,0xFF,0xFF,0xFF,0xF7 };

const ForceButtonsMap_t ForceButtonsMap[] {
{ FORCE_LAUNCH_1, 0 }, //  BT_MS1,
{ FORCE_LAUNCH_2, 0 }, //  BT_MS2,
{ FORCE_LAUNCH_3, 0 }, //  BT_MS3,
{ FORCE_LAUNCH_4, 0 }, //  BT_MS4,
{ FORCE_LAUNCH_5, 0 }, //  BT_MS5,
{ FORCE_LAUNCH_6, 0 }, //  BT_MS6,
{ FORCE_LAUNCH_7, 0 }, //  BT_MS7,
{ FORCE_LAUNCH_8, 0 }, //  BT_MS8,
{ FORCE_UP, 0 }, //  BT_UP,
{ FORCE_DOWN, 0 }, //  BT_DOWN,
{ FORCE_LEFT, 0 }, //  BT_LEFT,
{ FORCE_RIGHT, 0 }, //  BT_RIGHT,
{ FORCE_NAVIGATE, 0 }, //  BT_CLIP,
{ -1, 0 }, //  BT_MODE1,
{ -1, 0 }, //  BT_MODE2,
{ -1, 0 }, //  BT_SET,
{ FORCE_MENU, 0 }, //  BT_VOLUME,
{ -1, 0 }, //  BT_SENDA,
{ -1, 0 }, //  BT_SENDB,
{ -1, 0 }, //  BT_PAN,
{ -1, 0 }, //  BT_CONTROL1,
{ -1, 0 }, //  BT_CONTROL2,
{ -1, 0 }, //  BT_CONTROL3,
{ -1, 0 }, //  BT_CONTROL4,
};

// Column Pads:
// 90 60-67 00/7f
// Mute Pads:
// 90 29-30 00/7f


// Force Pads .  Offet from left top corner (Kikpad is also leftop)


// // Encoders touch note on/off midi msg from 1 to 8
// static const uint8_t MPCEncTouchMidiValueMap[] = {
//   0x54, 0x55, 0x56, 0x57,
//   0x58, 0x59, 0x5A, 0x5B,
// }

// Corresponding colors
static const int8_t MPCPadsColors[] = {
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,

  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
  RED  ,GREEN,MAGENTA, BLUE, RED  ,GREEN,MAGENTA, BLUE,
};

// 2 group of 4 Qlinks
static uint8_t QLinkCurrentBank = 0 ;

// Tap led count (it is our clock)
static uint8_t TapLedCount = 0;

///////////////////////////////////////////////////////////////////////////////
// SEND a MPC mapped ENCODER CC midi message
///////////////////////////////////////////////////////////////////////////////
static void ForceQlinkSendCC(uint8_t encoder,uint8_t ccValue) {

  midiPacket_t pk = { .i = 0 };

  // Simulate Touch
  pk.packet[0] = 0x09 ;
  pk.packet[1] = 0x90 ;
  pk.packet[2] = 0x53 + encoder ;
  pk.packet[3] = 0x7F ;
  MidiUSB.writePacket(&pk.i);

  // Send CC value
  pk.packet[0] = 0x0B ;
  pk.packet[1] = 0xB0 ;
  pk.packet[2] = 0x10 + encoder ;
  pk.packet[3] = ccValue ;
  MidiUSB.writePacket(&pk.i);

  // Simulate untouch
  pk.packet[0] = 0x08 ;
  pk.packet[1] = 0x80 ;
  pk.packet[2] = 0x53 + encoder ;
  pk.packet[3] = 00 ;
  MidiUSB.writePacket(&pk.i);
}

///////////////////////////////////////////////////////////////////////////////
// SEND A MPC Mapped Button Note ON on cable 0
///////////////////////////////////////////////////////////////////////////////
static void ForceButtonSend(uint8_t value,boolean pressed) {

  midiPacket_t pk = { .i = 0 };

  pk.packet[0] = 0x0B ;
  pk.packet[1] = 0xB0 ;
  pk.packet[2] = value ;
  pk.packet[3] = ( pressed ? 0x7F:0x00 ) ;

  MidiUSB.writePacket(&pk.i);
}

// Pad On:
// 99 36-75 00-7f
// Pad Off:
// 89 36-75 00
// Pad Pressure:
// a9 36-75 00-7f

///////////////////////////////////////////////////////////////////////////////
// Kikpad pads as Force pads
///////////////////////////////////////////////////////////////////////////////
static void ForcePadSend(uint8_t value,boolean pressed) {

  midiPacket_t pk = { .i = 0 };

  if ( pressed) {
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x99 ;
    pk.packet[3] = KIPAD_PAD_DEFAULT_VELOCITY ;

  } else {

    pk.packet[0] = 0x08 ;
    pk.packet[1] = 0x89 ;
    pk.packet[3] = 0 ;

  }
  pk.packet[2] = value ;

  MidiUSB.writePacket(&pk.i);
}

///////////////////////////////////////////////////////////////////////////////
// SET A PAD COLOR FROM RGB values
///////////////////////////////////////////////////////////////////////////////
void SetPadColorRGB(uint8_t pad, uint8_t r, uint8_t g, uint8_t b ) {
//6 bits xxrgbRGB (EGA)
// 128 / 4 values
// R Value 00 to 11
uint8_t R = r / (128/4) ;
uint8_t G = g / (128/4) ;
uint8_t B = b / (128/4) ;

uint8_t c = ( ( R & 0B10 ) << 1 ) + ( R & 0B01 << 5 ) +
            ( ( G & 0B10 )  ) + ( G & 0B01 << 4 ) +
            ( ( B >> 1 )  ) + ( B & 0B01 << 3 );

PadSetColor( pad, c);

}
///////////////////////////////////////////////////////////////////////////////
// PARSE SYSEX
///////////////////////////////////////////////////////////////////////////////
void SysEx_Parse(uint8_t byte)
{
  static unsigned sxMsgIdx = 0;
  static uint8_t sysexBuff[32];

  boolean match = false;

  match = ( byte == MPCSysexPadColor[sxMsgIdx] ) || ( MPCSysexPadColor[sxMsgIdx] == 0xFF ) || match ;

  if ( match ) {
    sysexBuff[sxMsgIdx] = byte;

    // F0 47 7F [3B] 65 00 04 [Pad #] [R] [G] [B] F7
    if ( ++sxMsgIdx == sizeof(MPCSysexPadColor) ) {
      SetPadColorRGB(sysexBuff[7],sysexBuff[8],sysexBuff[9],sysexBuff[10]);

      sxMsgIdx = 0;
      return;
    }

  } else sxMsgIdx = 0;


}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{

  uint8_t cin   = pk->packet[0] & 0x0F ;

  if ( cin == 0x0B ) {

      if ( pk->packet[1] == 0xB0 ) {

          switch (pk->packet[2]) {

            // TAP led
            case 0x35:
              // Tap led on = 1 beat
              if ( pk->packet[3] == 0x03 ) {
                ButtonSetLed(BT_VOLUME, ON);
              }
              else {
                ButtonSetLed(BT_VOLUME, OFF);
              }

              break;
          }
      }
  }
  else
  // SYSEX
  if ( cin > 3 &&  cin < 8 ) {
     // CIN 5 exception : tune request
    if (pk->packet[1] == 0XF6) return ;

    uint8_t pklen = ( cin == 4 ? 3 : cin - 4) ;
    for ( uint8_t i = 0 ; i< pklen ; i++ ) {
        SysEx_Parse(pk->packet[ i + 1 ]) ;
    }


  }

  return;

}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USER EVENT
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_ProcessUserEvent(UserEvent_t *ev){

  // Compute time between 2 events
  static unsigned long lastEventMicros = micros();
  unsigned long eventDelayMicros = micros() - lastEventMicros;
  lastEventMicros = micros();

  // To keep track of last event
  static uint8_t lastEventId  = EV_NONE;
  static uint8_t lastEventIdx = 0;

  static uint8_t encoderVal[8] = {0,0,0,0,0,0,0,0};

  midiPacket_t pk;


  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise (turn right)
    case EV_EC_CW:
    // Encoders Counter Clock wise (turn left)
    case EV_EC_CCW:
      {
        // Compute accelerator factor regarding the turn speed.
        uint8_t accel = 50 * 1000 / eventDelayMicros ;

        // Send the relative value
        if ( ev->ev == EV_EC_CCW )
            ForceQlinkSendCC(idx, 0x7f - accel);
        else
            ForceQlinkSendCC(idx, 1 + accel);

        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
    // Pad released
    case EV_PAD_RELEASED:
      {
        ForcePadSend( FORCE_PAD_OFFSET + idx, (ev->ev == EV_PAD_PRESSED) );

        break;
      }

    // Button pressed and not released
    case EV_BTN_PRESSED:
        if ( idx == BT_SET ) PadColorsBackground(BLACK);

      break;

    // Button released
    case EV_BTN_RELEASED:

      if ( ForceButtonsMap[idx].ctrl >= 0 ) {
          if ( ForceButtonsMap[idx].type == 0 ) { // Button
            // Press and release
            ForceButtonSend(ForceButtonsMap[idx].ctrl,true);
            ForceButtonSend(ForceButtonsMap[idx].ctrl,false);
          }
      }

      break;

    // Button pressed and holded more than 2s
    case EV_BTN_HOLDED:

      break;

  }

  // Keep track of last event
  lastEventId = ev->ev;
  lastEventIdx = idx;

}

///////////////////////////////////////////////////////////////////////////////
// Kikpad module setup
///////////////////////////////////////////////////////////////////////////////
static void KikpadMod_Setup() {


  ButtonsLedStates[1] = BTMSK_MS1 ;
  PadColorsBackground(BLACK);
  memcpy(PadColorsCurrent,MPCPadsColors,sizeof(MPCPadsColors));
  RGBMaskUpdateAll();

  PadLedStates[0] = PadLedStates[1] = 0XFFFFFFFF ;

}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
static void KikpadMod_Loop() {

}
#endif
