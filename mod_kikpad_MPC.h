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

// KIKPAD_MPC : MPC control surface emulation

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{
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

  enum MPCControls {
  // Pads first and enum starts at 0 !
    MPAD1  , MPAD2 , MPAD3 , MPAD4 ,
    MPAD5  , MPAD6 , MPAD7 , MPAD8 ,
    MPAD9  , MPAD10, MPAD11, MPAD12,
    MPAD13 , MPAD14, MPAD15, MPAD16,
    _END_MPADS,
  // Buttons
    MSHIFT,     MMENU,        MMAIN,       MUNDO,
    MCOPY ,     MTAP,         MREC,        MOVERDUB,
    MSTOP ,     MPLAY,        MPLAY_START, MPLUS,
    MMINUS,     MNOTE,        MFULL_LEVEL, MLEVEL_16,
    MERASE,     MPAD_BANKA,   MPAD_BANKB,  MPAD_BANKC,
    MPAD_BANKD, MQLINK_SELECT,
    _END_MBUTTONS,

    MNONE = 0xFF,
  } ;

  // Pads & simulated buttons note on/off message value
  static const uint8_t MPCPadMidiValueMap[] = {
    // Pads
    0x25, 0x24, 0x2A, 0X52,
    0x28, 0x26, 0x2E, 0x2C,
    0x30, 0x2F, 0x2D, 0x2B,
    0x31, 0x37, 0x33, 0x35,
    0xFF,// end pad

    // Buttons
    0x31, 0x7B, 0x34, 0X43,
    0x7A, 0x35, 0x49, 0X50,
    0x51, 0x52, 0x53, 0x36,
    0x37, 0x0B, 0x27, 0x28,
    0x09, 0x23, 0x24, 0x25,
    0x26, 0x00,
    0xFF, // end buttons
  };

  // // Encoders touch note on/off midi msg from 1 to 8
  // static const uint8_t MPCEncTouchMidiValueMap[] = {
  //   0x54, 0x55, 0x56, 0x57,
  //   0x58, 0x59, 0x5A, 0x5B,
  // }

  // Kipad MPC control affectations
  static const uint8_t MPCPadsMap[] = {
    MNONE,  MNONE,  MNONE,  MNONE,  MNONE, MNONE, MNONE, MNONE,
    MNONE,  MNONE,  MNONE,  MNONE,  MNONE, MNONE, MNONE, MNONE,
    MNONE,  MNONE,  MNONE,  MNONE,  MNONE, MNONE, MNONE, MNONE,
    MNONE,  MNONE,  MNONE,  MNONE,  MNONE, MNONE, MNONE, MNONE,
    MPAD13, MPAD14, MPAD15, MPAD16, MNONE, MNONE, MNONE, MNONE,
    MPAD9,  MPAD10, MPAD11, MPAD12, MNONE, MNONE, MNONE, MNONE,
    MPAD5,  MPAD6,  MPAD7,  MPAD8,  MNONE, MOVERDUB, MNONE, MPLAY_START,
    MPAD1,  MPAD2,  MPAD3,  MPAD4,  MNONE, MREC , MSTOP, MPLAY,
  };

  // Corresponding colors
  static const int8_t MPCPadsColors[] = {
    BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
    BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
    BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
    BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
    RED  ,RED  ,RED  ,RED  ,BLACK,BLACK,BLACK,BLACK,
    RED  ,RED  ,RED  ,RED  ,BLACK,BLACK,BLACK,BLACK,
    RED  ,RED  ,RED  ,RED  ,BLACK,MAGENTA,BLACK,YELLOW,
    RED  ,RED  ,RED  ,RED  ,BLACK,RED,  BLUE, GREEN,
  };

  static boolean setupMode = true;
  static uint8_t currentSendMode=0;
  static const uint8_t encoderCC[8]={CC_VOL,CC_EFFECT1,CC_EFFECT2,CC_PAN,CC_CUTOFF,CC_RESO,0,0};
  static uint8_t encoderVal[8] = {0,0,0,0,0,0,0,0};
  static uint8_t midiChTranspose = 0;
  static uint8_t midiChannel = 0;
  static int8_t midiNoteTranspose = 0;

  midiPacket_t pk;

  if (setupMode ) {
      ButtonsLedStates[0] = BTMSK_VOLUME ;
      ButtonsLedStates[1] = BTMSK_MS1 ;
      PadColorsBackground(BLACK);
      memcpy(PadColorsCurrent,MPCPadsColors,sizeof(MPCPadsColors));
      RGBMaskUpdateAll();

      PadLedStates[0] = PadLedStates[1] = 0XFFFFFFFF ;
      currentSendMode = 0;
      midiChannel = 0;
      setupMode = false;
      if ( !ev ) return;
  }

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise
    case EV_EC_CW:

      if ( ++encoderVal[idx] > 127 ) encoderVal[idx] = 127;

      // MPC Encoder simulate touch
      pk.packet[0] = 0x09 ;
      pk.packet[1] = 0x90 ;
      pk.packet[2] = 0X54 + idx ;
      pk.packet[3] = 0x7F ;
      MidiUSB.writePacket(&pk.i);

      // MPC Encoder rotate right : B0 10 01
      pk.packet[0] = 0x0B ;
      pk.packet[1] = 0xB0 ;
      pk.packet[2] = 0X10 + idx ;
      pk.packet[3] = 0x01 ;
      MidiUSB.writePacket(&pk.i);

      // MPC Encoder simulate untouch
      pk.packet[0] = 0x09 ;
      pk.packet[1] = 0x90 ;
      pk.packet[2] = 0X54 + idx ;
      pk.packet[3] = 0x00 ;
      MidiUSB.writePacket(&pk.i);

      break;

    // Encoders Counter Clock wise
    case EV_EC_CCW:
      if ( encoderVal[idx] > 0 ) encoderVal[idx]--;

      // MPC Encoder simulate touch
      pk.packet[0] = 0x09 ;
      pk.packet[1] = 0x90 ;
      pk.packet[2] = 0X54 + idx ;
      pk.packet[3] = 0x7F ;
      MidiUSB.writePacket(&pk.i);

      // MPC Encoder rotate left : B0 10 7F
      pk.packet[0] = 0x0B ;
      pk.packet[1] = 0xB0 ;
      pk.packet[2] = 0X10 + idx ;
      pk.packet[3] = 0x7F ;
      MidiUSB.writePacket(&pk.i);

      // MPC Encoder simulate untouch
      pk.packet[0] = 0x09 ;
      pk.packet[1] = 0x90 ;
      pk.packet[2] = 0X54 + idx ;
      pk.packet[3] = 0x00 ;
      MidiUSB.writePacket(&pk.i);

      break;

    // Pad pressed and not released
    case EV_PAD_PRESSED:
      if ( MPCPadsMap[idx] < _END_MBUTTONS ) {
        PadColorsBackup[idx] = PadColorsCurrent[idx];
        PadSetColor(idx, WHITE);
        if ( MPCPadsMap[idx] < _END_MPADS )
          pk.packet[1] = 0x99;
        else
          pk.packet[1] = 0x90; // Pad simulating a button

        pk.packet[0] = 0x09;
        pk.packet[2] = MPCPadMidiValueMap[MPCPadsMap[idx]];
        pk.packet[3] = 0x7F;
        MidiUSB.writePacket(&pk.i);
      }
      break;

    // Pad released
    case EV_PAD_RELEASED:
      if ( MPCPadsMap[idx] < _END_MBUTTONS ) {
        PadSetColor(idx, PadColorsBackup[idx]);
        if ( MPCPadsMap[idx] < _END_MPADS )
          pk.packet[1] = 0x89;
        else
          pk.packet[1] = 0x80; // Pad simulating a button

        pk.packet[0] = 0x08;
        pk.packet[2] = MPCPadMidiValueMap[MPCPadsMap[idx]];
        pk.packet[3] = 0x00;
        MidiUSB.writePacket(&pk.i);
      }
      break;

    // Button pressed and not released
    case EV_BTN_PRESSED:


      break;

    // Button released
    case EV_BTN_RELEASED:
      if ( idx >= BT_VOLUME && idx <= BT_CONTROL4 ) {
        // Switch off all buttons of bank 0, left bar
        ButtonsLedStates[0] &= 0xFF000000;
        ButtonSetLed(idx, ON);
        currentSendMode = idx - BT_VOLUME;
      }
      else
      if ( idx >= BT_MS1 && idx <= BT_MS8 ) {
        // Switch off all buttons of bank 1, right bar
        ButtonsLedStates[1] = 0;
        midiChannel = idx - BT_MS1;
        ButtonSetLed(idx, ON);
      }
      else
      if ( idx == BT_UP ) {
        if ( midiChTranspose ) {
          midiChTranspose = 0;
          ButtonSetLed(idx, OFF);
        } else {
          midiChTranspose = 8;
          ButtonSetLed(idx, ON);
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
#endif
