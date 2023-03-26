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
  ------------------------------------------------------------------------------
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

// KIKPAD LaunchPadMK3 : Make the Smartpad a Launchpad MK3

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_

// We have a pre setup in that module to change usb name
#define MODULE_PRE_SETUP

#define ENCODERS_RELATIVE
#define KIPAD_PAD_DEFAULT_VELOCITY 0x7F

#define LP_DEVICE_PRO_MK3 0x0E
#define LP_DEVICE_MINI_MK3 0x0D
#define LP_DEVICE LP_DEVICE_MINI_MK3

// Usb settings
// 1235:0113 Focusrite-Novation
#define LP_VID 0x1235
#define LP_PID 0x0113
#define LP_PRODUCT_STRING "Launchpad Mini MK3"
#define LP_SERIAL "AYCY4WP1B12345"

#define LP_LAYOUT_PGM_MODE 0x7F
#define LP_CC_FADER 0xB4
#define LP_FADER_OFFSET 0x00

#define LP_FIRMWARE_VERSION 0x00,0x03,0x04,0x01
#define LP_SYSEX_HEADER 0xF0,0x00,0x20,0x29,0x02,LP_DEVICE

static const uint8_t LpSx_UniversalDeviceInquiry[] = { 0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7 };
static const uint8_t LpSx_DeviceInquiryReply[] { 0xF0, 0x7E, 0x00, 0x06, 0x02, 0x00, 0x20, 0x29, 0x13, 0x01, 0x00, 0x00, LP_FIRMWARE_VERSION, 0xF7 };

// Set Led color
static const uint8_t  LpSX_SetPadColorHeader[] = {LP_SYSEX_HEADER, 0x03 };

// Daw mode enable/disable
static const uint8_t  LpSX_EnableDawModeHeader[] = {LP_SYSEX_HEADER,0x10 } ;

// Select Layout
static const uint8_t  LpSX_SelectLayoutHeader[] = {LP_SYSEX_HEADER,0x00 } ;

// Standalone / Daw mode
static uint8_t DawModeEnabled = 0;
// Current Layout
static uint8_t CurrentLayout = 0x7F; // Programmer mode


// CC buttons around the pads.
// CC are those of the Launchpad Mini remapped with the Smartpad when possible
enum LaunchpadButtonsCtrlValue {
  // Top buttons line on the Mini
  LP_UP = 0x5B,LP_DOWN = 0x5C,LP_LEFT = 0x5D,LP_RIGHT = 0x5E,LP_SESSION = 0x5F,LP_DRUMS = 0x60,LP_KEYS = 0x61,LP_USER = 0x62,
  // Right vertical buttons line on the Mini
  LP_LAUNCH_1 = 0x59,LP_LAUNCH_2 = 0x4F, LP_LAUNCH_3 = 0x45,LP_LAUNCH_4 = 0x3B,LP_LAUNCH_5 = 0x31, LP_LAUNCH_6 = 0x27, LP_LAUNCH_7 = 0x1D, LP_LAUNCH_8 = 0x13,
  // LEft vertical buttons line on the Pro (nothing on the mini), from top to bottom
  LP_LEFT_1 = 0x50, LP_LEFT_2 = 0x46, LP_LEFT_3 = 0x3C, LP_LEFT_4 = 0x32, LP_LEFT_5 = 0x28, LP_LEFT_6 = 0x1E, LP_LEFT_7 = 0x14, LP_LEFT_8 = 0x0A,
};

// Control mapping
typedef struct{
  int8_t ctrl;
  int8_t led; // Associated button led index if different
}  __packed LPButtonsMap_t;

const LPButtonsMap_t LPButtonsMap[] {
{ LP_LAUNCH_1, -1 }, //  BT_MS1,
{ LP_LAUNCH_2, -1 }, //  BT_MS2,
{ LP_LAUNCH_3, -1 }, //  BT_MS3,
{ LP_LAUNCH_4, -1 }, //  BT_MS4,
{ LP_LAUNCH_5, -1 }, //  BT_MS5,
{ LP_LAUNCH_6, -1 }, //  BT_MS6,
{ LP_LAUNCH_7, -1 }, //  BT_MS7,
{ LP_LAUNCH_8, -1 }, //  BT_MS8,
{ LP_UP,       -1 }, //  BT_UP,
{ LP_DOWN,     -1 }, //  BT_DOWN,
{ LP_LEFT,     -1 }, //  BT_LEFT,
{ LP_RIGHT,    -1 }, //  BT_RIGHT,
{ LP_SESSION,  -1 }, //  BT_CLIP,
{ LP_DRUMS,    -1 }, //  BT_MODE1,
{ LP_KEYS,     -1 }, //  BT_MODE2,
{ LP_USER,     -1 }, //  BT_SET,
{ LP_LEFT_1,   -1 }, //  BT_VOLUME,
{ LP_LEFT_2,   -1 }, //  BT_SENDA,
{ LP_LEFT_3,   -1 }, //  BT_SENDB,
{ LP_LEFT_4,   -1 }, //  BT_PAN,
{ LP_LEFT_5,   -1 }, //  BT_CONTROL1,
{ LP_LEFT_6,   -1 }, //  BT_CONTROL2,
{ LP_LEFT_7,   -1 }, //  BT_CONTROL3,
{ LP_LEFT_8,   -1 }, //  BT_CONTROL4,
};
// Inverted table
uint8_t LPButtonsMap_Inv[128];

//////////////////////////////////////////////////////////////////////////////
// Send a SYSEX midi message to USB Cable n
///////////////////////////////////////////////////////////////////////////////
boolean USBMidi_SendSysExMsg( uint8_t cable, const uint8_t sxBuff[],uint16_t sz)
{
  midiPacket_t pk { .i = 0};
  uint8_t b=0;
  bool startSx=false;
  bool endSx=false;

  if (cable > 0x0F) return false;
  if ( sxBuff[0] != 0xF0 || sxBuff[sz-1] != 0xF7) return false;

  // Build sysex packets
  // Multiple Sysyex messages can be embedded in the buffer :
  // F0 nn ... nn F7 F0 nn ... nn F7 so we have to care about that.

  for ( uint16_t i = 0; i != sz ; i ++ ) {
      // Check integrity
      if ( sxBuff[i] == 0xF0) startSx = true;
      if ( sxBuff[i] == 0xF7) endSx = true;
      if (startSx) {
        pk.packet[++b] = sxBuff[i];
        if ( b == 3 || endSx ) {
          pk.packet[0]  = (endSx ?  b + 4 : 4 ) + (cable << 4);
          MidiUSB.writePacket(&pk.i);
          if (endSx) startSx = endSx = false;
          b=0; pk.i = 0;
        }
      }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// SEND a knob fader mapped CC midi message
///////////////////////////////////////////////////////////////////////////////
// 8 faders.
static void LPKnobSendCC(uint8_t knob,uint8_t ccValue) {

  midiPacket_t pk = { .i = 0 };

  // Send CC value
  pk.packet[0] = 0x0B ;
  pk.packet[1] = LP_CC_FADER ;
  pk.packet[2] = LP_FADER_OFFSET + knob ;
  pk.packet[3] = ccValue ;
  MidiUSB.writePacket(&pk.i);
}

///////////////////////////////////////////////////////////////////////////////
// SEND A mapped Button Note ON on cable 0
///////////////////////////////////////////////////////////////////////////////
static void LPButtonSend(uint8_t value,boolean pressed) {

  midiPacket_t pk = { .i = 0 };

  pk.packet[0] = 0x0B ;
  pk.packet[1] = 0xB0 ;
  pk.packet[2] = value ;
  pk.packet[3] = ( pressed ? 0x7F:0x00 ) ;

  MidiUSB.writePacket(&pk.i);
}

///////////////////////////////////////////////////////////////////////////////
// Kikpad pads as LP pads
///////////////////////////////////////////////////////////////////////////////
static void LPPadSend(uint8_t value,boolean pressed) {

  midiPacket_t pk = { .i = 0 };

  if ( pressed) {
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x90 ;
    pk.packet[3] = KIPAD_PAD_DEFAULT_VELOCITY ;

  } else {

    pk.packet[0] = 0x08 ;
    pk.packet[1] = 0x80 ;
    pk.packet[3] = 0 ;

  }
  pk.packet[2] = value ;

  MidiUSB.writePacket(&pk.i);
}

///////////////////////////////////////////////////////////////////////////////
// SET A PAD COLOR FROM RGB values
///////////////////////////////////////////////////////////////////////////////
void SetPadColorRGB(uint8_t pad, uint8_t r, uint8_t g, uint8_t b ) {
// 6 bits xxrgbRGB (EGA)
// Value are from 0 to 127 => 0 to 3

r /= 42.333 ;
g /= 42.333 ; 
b /= 42.333 ;

r = ( ( r & 0B10 ) << 1 ) + ( ( r & 0B1 ) << 5 );
g = ( ( g & 0B10 )      ) + ( ( g & 0B1 ) << 4 );
b = ( ( b & 0B10 ) >> 1 ) + ( ( b & 0B1 ) << 3 );


PadSetColor( pad, r + g + b);

}
///////////////////////////////////////////////////////////////////////////////
// PARSE SYSEX
///////////////////////////////////////////////////////////////////////////////
void SysEx_Parse(uint8_t byte)
{
  static unsigned sxLen = 0;

  static unsigned sxMsgIdx = 0 ;
  static uint8_t sysexBuff[64];

  // First byte ?
  if ( byte == 0xF0) sxLen = 0 ;


  // Overflow. Drop everything until the next sysex start
  if ( sxLen >= sizeof(sysexBuff) ) return ;

  sysexBuff[sxLen++] = byte;

  // was Last byte ?
  if ( byte != 0xF7) return ;

  // Device inquiry (test optimized). Reply to the 2 ports
  if ( sysexBuff[1] == 0x7E && memcmp( sysexBuff, LpSx_UniversalDeviceInquiry, sizeof(LpSx_UniversalDeviceInquiry) ) == 0 ) {
    USBMidi_SendSysExMsg( 0, LpSx_DeviceInquiryReply,sizeof(LpSx_DeviceInquiryReply) );
    USBMidi_SendSysExMsg( 1, LpSx_DeviceInquiryReply,sizeof(LpSx_DeviceInquiryReply) );
    sxLen = 0 ;
  }

  // Daw mode
  else
  if ( sysexBuff[6] == 0x10 && memcmp( sysexBuff, LpSX_EnableDawModeHeader, sizeof(LpSX_EnableDawModeHeader) ) == 0 ) {

      // Request current mode ?
      if ( sysexBuff[7] == 0xF7 ) {
        sysexBuff[7] = DawModeEnabled;
        sysexBuff[8] = 0xF7;
        USBMidi_SendSysExMsg( 0, sysexBuff,sxLen + 1 );
        USBMidi_SendSysExMsg( 0, sysexBuff,sxLen + 1 );
      }
      // Set Mode
      else {
        DawModeEnabled = sysexBuff[7] ;
      }
      sxLen = 0 ;
  }

  // Select Layout
  else
  if ( sysexBuff[6] == 0x00 && memcmp( sysexBuff, LpSX_SelectLayoutHeader, sizeof(LpSX_SelectLayoutHeader) ) == 0 ) {
      // Request current layout ?
      if ( sysexBuff[7] == 0xF7 ) {
        sysexBuff[7] = CurrentLayout;
        sysexBuff[8] = 0xF7;
        USBMidi_SendSysExMsg( 0, sysexBuff,sxLen + 1 );
        USBMidi_SendSysExMsg( 0, sysexBuff,sxLen + 1 );
      }
      // Set Mode
      else {
        CurrentLayout = sysexBuff[7] ;
      }
      sxLen = 0 ;
  }

  // Set Led Color
  // F0h 00h 20h 29h 02h 0Eh 03h <Colour Spec> [ <Colour Spec> [_] ] F7h
  // The <Colour Spec> is structured as follows:
  // - Lighting type (1 byte) LED index (1 byte) Lighting data (1 – 3 bytes)
  // 00h : 0 --- Static colour from palette, Lighting data is 1 byte specifying palette entry.
  // 01h : 1 --- Flashing colour, Lighting data is 2 bytes specifying Colour B and Colour A.
  // 02h : 2 --- Pulsing colour, Lighting data is 1 byte specifying palette entry.
  // 03h : 3 --- RGB colour, Lighting data is 3 bytes for Red, Green and Blue (127:Max, 0: Min).
  // F0 00 20 29 02 0D 03 03  0B  7F 7F 7F F7

  //"F0 00 20 29 02 0D 03  00 3A 7F  01 3A 40 41  02 3A 41  03 0B 7F 7F 7F   F7"
  else
  if ( sysexBuff[6] == 0x03 && memcmp( sysexBuff, LpSX_SetPadColorHeader, sizeof(LpSX_SetPadColorHeader) ) == 0 )  {

    uint8_t idx = 7; // Start at <Colour Spec>
    while ( idx < sxLen ) {
      // Button or pad led ?
      uint8_t padL = sysexBuff[idx + 1 ] / 10 ;
      uint8_t padC = sysexBuff[idx + 1 ] % 10 ;
      bool isPad = ( padL > 0 && padL < 9 && padC > 0 && padC < 9);

      if ( isPad) {
        padL-- ; padC-- ;
        uint8_t pad = ( 7 - padL ) * 8 + padC;
        switch (sysexBuff[idx]) {
          case 0: // Static color
            PadSetColor( pad, sysexBuff[idx + 2] );
            idx += 3;
            break;
          case 1: // Flash
            idx += 4;
            break;
          case 2: // Pulsing
            idx += 3;
            break;
          case 3: // RGB
            SetPadColorRGB(pad,sysexBuff[idx + 2],sysexBuff[idx + 3],sysexBuff[idx + 4]);
            idx += 5;
            break;
        }
      }
      // Button Led mapped ?
      else {
        int16_t b = LPButtonsMap_Inv[ sysexBuff[idx + 1 ] ];
        // The Smartpad has no colors for buttons. Only on/off
        switch (sysexBuff[idx]) {
          case 0: // Static color
            if ( b >= 0 ) ButtonSetLed(  b  , sysexBuff[idx+2] == 0 ? OFF : ON);
            idx += 3;
            break;
          case 1: // Flash
            idx += 4;
            break;
          case 2: // Pulsing
            idx += 3;
            break;
          case 3: // RGB
            if ( b >= 0 ) {
              if ( (sysexBuff[idx + 2] + sysexBuff[idx + 3] + sysexBuff[idx + 4]) != 0 ) {
                ButtonSetLed(  b  , ON);
              }
              else ButtonSetLed(  b  , OFF);
            }
            idx += 5;
            break;
        }
      }
      if ( sysexBuff[idx] == 0xF7 ) break;
    }

    sxLen = 0 ;
  }

}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{
  uint8_t cin   = pk->packet[0] & 0x0F ;

  // Lighting leds - Pads
  if ( cin == 0x09 ) {

      uint8_t padL = pk->packet[2] / 10 ;
      uint8_t padC = pk->packet[2] % 10 ;
      bool isPad = ( padL > 0 && padL < 9 && padC > 0 && padC < 9);
      if ( isPad ) {
        padL -- ;  padC -- ;
        uint8_t pad = ( 7 - padL ) * 8 + padC;
        // Static colour
        if ( pk->packet[1] == 0x90 ) {
          PadSetColor( pad, pk->packet[3] );
        }
        // Flashing colour
        else if ( pk->packet[1] == 0x91 ) {
        }
        // Pulsing Colour
        else if ( pk->packet[1] == 0x92 ) {
        }
      }
  }

  // Lighting Buttons
  else
  if ( cin == 0x0B ) {
    uint8_t padL = pk->packet[2] / 10 ;
    uint8_t padC = pk->packet[2] % 10 ;
    bool isPad = ( padL > 0 && padL < 9 && padC > 0 && padC < 9);
    if ( !isPad ) {
      int16_t b = LPButtonsMap_Inv[ pk->packet[2] ];
      // Static colour
      if ( pk->packet[1] == 0xB0 ) {
        if ( b >= 0 ) ButtonSetLed(  b  , pk->packet[3] != 0 ? ON:OFF);
      }
      // Flashing colour
      else if ( pk->packet[1] == 0xB1 ) {
      }
      // Pulsing Colour
      else if ( pk->packet[1] == 0xB2 ) {
      }
    }
  }

  // SYSEX
  else
  if ( cin > 3 &&  cin < 8 ) {
     // CIN 5 exception : tune request
    if (pk->packet[1] == 0XF6) return ;

    uint8_t pklen = ( cin == 4 ? 3 : cin - 4) ;
    for ( uint8_t i = 0 ; i< pklen ; i++ ) {
        SysEx_Parse(pk->packet[ i +  1 ]) ;
    }
  }

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
            LPKnobSendCC(idx, 0x7f - accel);
        else
            LPKnobSendCC(idx, 1 + accel);

        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
    // Pad released
    case EV_PAD_RELEASED:
      {
        // Kipad number from top left
        // Lauchpad from bottom left to up
        uint8_t pad = ( 7 - idx / 8 )*10 + idx % 8 + 0x0B ;

        LPPadSend( pad, (ev->ev == EV_PAD_PRESSED) );


        break;
      }

    // Button pressed and not released
    case EV_BTN_PRESSED:
    // Button released
    case EV_BTN_RELEASED:

        //if ( idx == BT_SET ) PadColorsBackground(BLACK);
        LPButtonSend(LPButtonsMap[idx].ctrl,(ev->ev == EV_BTN_PRESSED));

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

static void KikpadMod_Pre_Setup() {
  usb_midi_set_product_string(LP_PRODUCT_STRING);
  usb_midi_set_vid_pid(LP_VID, LP_PID);
}

// Post Main Kikpad setup (after usb init)
static void KikpadMod_Setup() {

  PadColorsBackground(BLACK);

  RGBMaskUpdateAll();

  //PadLedStates[0] = PadLedStates[1] = 0XFFFFFFF ;

  // Build button map inverted table
  for ( uint8_t i = 0 ; i < sizeof(LPButtonsMap_Inv) ; i++ ) {
    LPButtonsMap_Inv[ i ] =  -1;
  }
  for ( uint8_t i = 0 ; i < sizeof(LPButtonsMap) / sizeof(LPButtonsMap_t) ; i++ ) {
    LPButtonsMap_Inv[ LPButtonsMap[i].ctrl ] =  i;
  }

}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
static void KikpadMod_Loop() {

}
#endif
