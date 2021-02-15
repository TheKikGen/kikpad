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

// KIKPAD_MPCClips : MPC control surface emulation - 64 Clips launcher

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_


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
  MPAD_BANKD, MQLINK_SELECT,SAMPLE_EDIT, PAD_MIXER,
  CH_MIXER,
  _END_MBUTTONS,

  MNONE = 0xFF,
} ;

// Pads & simulated buttons note on/off message value
static const uint8_t MPCPadMidiValueMap[] = {
  // Pads.  0x99
  0x25, 0x24, 0x2A, 0X52,
  0x28, 0x26, 0x2E, 0x2C,
  0x30, 0x2F, 0x2D, 0x2B,
  0x31, 0x37, 0x33, 0x35,
  0xFF,// end pad

  // Buttons. 0x90
  0x31, 0x7B, 0x34, 0X43,
  0x7A, 0x35, 0x49, 0X50,
  0x51, 0x52, 0x53, 0x36,
  0x37, 0x0B, 0x27, 0x28,
  0x09, 0x23, 0x24, 0x25,
  0x26, 0x00, 0x06, 0x73,
  0x74,
  0xFF, // end buttons
};

// // Encoders touch note on/off midi msg from 1 to 8
// static const uint8_t MPCEncTouchMidiValueMap[] = {
//   0x54, 0x55, 0x56, 0x57,
//   0x58, 0x59, 0x5A, 0x5B,
// }

// Kipad MPC control affectations
static const uint8_t MPCPadsMap[] = {
  MPAD13, MPAD14, MPAD15, MPAD16, MPAD13, MPAD14, MPAD15, MPAD16,
  MPAD9,  MPAD10, MPAD11, MPAD12, MPAD9,  MPAD10, MPAD11, MPAD12,
  MPAD5,  MPAD6,  MPAD7,  MPAD8,  MPAD5,  MPAD6,  MPAD7,  MPAD8,
  MPAD1,  MPAD2,  MPAD3,  MPAD4,  MPAD1,  MPAD2,  MPAD3,  MPAD4,
  MPAD13, MPAD14, MPAD15, MPAD16, MPAD13, MPAD14, MPAD15, MPAD16,
  MPAD9,  MPAD10, MPAD11, MPAD12, MPAD9,  MPAD10, MPAD11, MPAD12,
  MPAD5,  MPAD6,  MPAD7,  MPAD8,  MPAD5,  MPAD6,  MPAD7,  MPAD8,
  MPAD1,  MPAD2,  MPAD3,  MPAD4,  MPAD1,  MPAD2,  MPAD3,  MPAD4,
};

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

// 4 group of 4 Qlinks
static uint8_t QLinkCurrentGroup = 0 ;
// 2 master groups of 8 qlinks
static uint8_t QLinkCurrentMasterGroup = 0 ;

// 4 banks of 16 pads, upddated with LED msg
static uint8_t PadCurrentBank = 0 ;

// Tap led count (it is our clock)
static uint8_t TapLedCount = 0;

// Clips Status On/Off
static uint8_t MPCClipsStatus[64];

///////////////////////////////////////////////////////////////////////////////
// Set Qlink group by simulating Qlink select button press
///////////////////////////////////////////////////////////////////////////////
static void SetQLinkCurrentGroup(uint8_t group) {

  midiPacket_t pk;

  if (group == QLinkCurrentGroup || group > 3  ) return;

  if ( group > QLinkCurrentGroup )
    group -=QLinkCurrentGroup;
  else
    group = 4 - QLinkCurrentGroup + group;

  for ( uint8_t i = 0 ; i < group ; i++  ) {
    // Simulate QLink select button
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x90 ;
    pk.packet[2] = 0X00 ;
    pk.packet[3] = 0x7F ;
    MidiUSB.writePacket(&pk.i);
    // Released
    pk.packet[3] = 0x00 ;
    MidiUSB.writePacket(&pk.i);
  }

}

///////////////////////////////////////////////////////////////////////////////
// Set Pad Bank by simulating pad bank select button press
///////////////////////////////////////////////////////////////////////////////
static void SetPadCurrentBank(uint8_t bank) {

  if ( bank == PadCurrentBank || bank > 7  ) return;

  midiPacket_t pk;

  // Compute the # of the bank button
  uint8_t bt  =  (bank > 3 ) ? bank - 4: bank;

  uint8_t  i = 1;
  // Check if we need a "double press"
  if ( bank > 3 && PadCurrentBank < 4 ) i++;

  for (  ; i > 0 ; i--  ) {
    // Simulate Pad select Bank button
    // 90 [23-26] 7F-00
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x90 ;
    pk.packet[2] = 0X23 + bt ;
    pk.packet[3] = 0x7F ;
    MidiUSB.writePacket(&pk.i);

    // Released
    pk.packet[3] = 0x00 ;
    MidiUSB.writePacket(&pk.i);
  }

}


///////////////////////////////////////////////////////////////////////////////
// Set Pad Bank by simulating pad bank select button press
///////////////////////////////////////////////////////////////////////////////
static void ClipsPadAnimate(uint8_t on) {

  for ( uint8_t i = 0 ; i < 64 ; i++ ) {

    if ( MPCClipsStatus[i] ) {

      if ( on) PadSetColor(i, MPCPadsColors[i]);
      else PadSetColor(i, BLACK);
    }

  }

}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{

  if ( pk->packet[0] == 0x0B ) {

      if ( pk->packet[1] == 0xB0 ) {

          switch (pk->packet[2]) {

            // TAP led
            case 0x35:
              // Tap led on = 1 beat
              if ( pk->packet[3] == 0x03 ) {
                if ( ++TapLedCount > 4) TapLedCount = 1;
                ButtonSetLed(BT_VOLUME, ON);
                if ( TapLedCount == 1 ) ClipsPadAnimate(1);
              }
              else {
                ButtonSetLed(BT_VOLUME, OFF);
                if ( TapLedCount == 3 ) ClipsPadAnimate(0);
              }

              break;

            // QLink select led
            // We use QLink led # to get the current QLink set set
            case 0x5A: case 0x5B: case 0x5C: case 0x5D:
              if ( pk->packet[3] == 0x03 ) QLinkCurrentGroup = pk->packet[2] - 0x5A;
              break;

            // Pad led bank
            case 0x23: case 0x24: case 0x25: case 0x26:
              // 00 : Off, 01:dim red, 02:dim orange, 03:red,04:orange
              if ( pk->packet[3] == 0x03 || pk->packet[3] == 0x04 )
                  PadCurrentBank = pk->packet[2] - 0x23 + 4 * (pk->packet[3] - 3 ) ;
              break;

          }
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


  static boolean setupMode = true;
  static uint8_t encoderVal[8] = {0,0,0,0,0,0,0,0};

  midiPacket_t pk;

  if (setupMode ) {
      setupMode = false;
      QLinkCurrentMasterGroup = 0 ;
      ButtonsLedStates[1] = BTMSK_MS1 ;
      PadColorsBackground(BLACK);
      memcpy(PadColorsCurrent,MPCPadsColors,sizeof(MPCPadsColors));
      RGBMaskUpdateAll();
      memset(MPCClipsStatus,0,sizeof(MPCClipsStatus));

      PadLedStates[0] = PadLedStates[1] = 0XFFFFFFFF ;
      if ( !ev ) return;
  }

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise
    case EV_EC_CW:
      {
        if ( ++encoderVal[idx] > 127 ) encoderVal[idx] = 127;

        SetQLinkCurrentGroup(idx / 4 + QLinkCurrentMasterGroup * 2);
        uint8_t i = idx % 4;

        // MPC Encoder simulate touch
        pk.packet[0] = 0x09 ;
        pk.packet[1] = 0x90 ;
        pk.packet[2] = 0X54 + i ;
        pk.packet[3] = 0x7F ;
        MidiUSB.writePacket(&pk.i);

        // MPC Encoder rotate right : B0 10 01
        pk.packet[0] = 0x0B ;
        pk.packet[1] = 0xB0 ;
        pk.packet[2] = 0X10 + i ;
        pk.packet[3] = 0x01 ;
        MidiUSB.writePacket(&pk.i);

        // MPC Encoder simulate untouch
        pk.packet[0] = 0x09 ;
        pk.packet[1] = 0x90 ;
        pk.packet[2] = 0X54 + i ;
        pk.packet[3] = 0x00 ;
        MidiUSB.writePacket(&pk.i);

        break;
      }

    // Encoders Counter Clock wise
    case EV_EC_CCW:
      {
        if ( encoderVal[idx] > 0 ) encoderVal[idx]--;

        SetQLinkCurrentGroup( idx / 4  + QLinkCurrentMasterGroup * 2);
        uint8_t i = idx % 4;

        // MPC Encoder simulate touch
        pk.packet[0] = 0x09 ;
        pk.packet[1] = 0x90 ;
        pk.packet[2] = 0X54 + i ;
        pk.packet[3] = 0x7F ;
        MidiUSB.writePacket(&pk.i);

        // MPC Encoder rotate left : B0 10 7F
        pk.packet[0] = 0x0B ;
        pk.packet[1] = 0xB0 ;
        pk.packet[2] = 0X10 + i ;
        pk.packet[3] = 0x7F ;
        MidiUSB.writePacket(&pk.i);

        // MPC Encoder simulate untouch
        pk.packet[0] = 0x09 ;
        pk.packet[1] = 0x90 ;
        pk.packet[2] = 0X54 + i ;
        pk.packet[3] = 0x00 ;
        MidiUSB.writePacket(&pk.i);

        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
      {

        // Set the bank regarding the pad idx
        // 4x16 pads in the MPC
        uint8_t padBank = (idx % 8 > 3 ) ? 1 : 0 ;
        if ( idx < 32 ) padBank += 2;
        SetPadCurrentBank(padBank);

        // Send the pad value
        //PadColorsBackup[idx] = PadColorsCurrent[idx];
        PadSetColor(idx, WHITE);
        pk.packet[0] = 0x09;
        pk.packet[1] = 0x99;
        pk.packet[2] = MPCPadMidiValueMap[MPCPadsMap[idx]];
        pk.packet[3] = 0x7F;
        MidiUSB.writePacket(&pk.i);

        // Set the Clip status. Not synched wiht MPC pads...
        // Need to decode sysex...later
        MPCClipsStatus[idx] = ! MPCClipsStatus[idx];

        break;
      }

    // Pad released
    case EV_PAD_RELEASED:
        //PadSetColor(idx, PadColorsBackup[idx]);
        PadSetColor(idx, MPCPadsColors[idx]);
        pk.packet[0] = 0x08;
        pk.packet[1] = 0x89;
        pk.packet[2] = MPCPadMidiValueMap[MPCPadsMap[idx]];
        pk.packet[3] = 0x00;
        MidiUSB.writePacket(&pk.i);
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
      }
      else
      if ( idx >= BT_MS1 && idx <= BT_MS8 ) {
        // Switch off all buttons of bank 1, right bar
        ButtonsLedStates[1] = 0;
        ButtonSetLed(idx, ON);

        if ( idx == BT_MS1 ) QLinkCurrentMasterGroup = 0;
        else if ( idx == BT_MS2 ) QLinkCurrentMasterGroup = 1;


      }
      else
      if ( idx == BT_UP ) {
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


}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
static void KikpadMod_Loop() {

}
#endif
