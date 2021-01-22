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

// KIKPAD_MPC : Advanced MPC midi controller

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_

#define MPC_PADS_MIDI_CHANNEL 0x00

#define MPC_CONTROL_MIDI_CHANNEL 0x0F

#define MPC_QLINKS_MIDI_CHANNEL MPC_CONTROL_MIDI_CHANNEL
#define MPC_BUTTON_MIDI_CHANNEL MPC_CONTROL_MIDI_CHANNEL

#define MPC_QLINKS_FIRST_CC_NUMBER 100


static boolean  MPCPlaying = false;
static boolean  MPCOverdub = false;
static uint16_t MPCBar   = 0;
static uint8_t  MPCBeat  = 0;
static uint16_t MPCClock = 0;
static uint8_t  MPCTick  = 0;
static unsigned long  MPCSongPointerPos = 0;      // Song Pointer Position

// 2 banks of 64 pads for Kikpad
static uint8_t KikpadBank = 0;

// 8 banks A-H of 16 MPC pads
static uint8_t MPCPadsBank = 0;
static const uint8_t KikpadPadsBankNotesMap[]={16,17,18,19,20,21,22,23};

// Current pads velocity
static uint8_t KikpadsVelocity = 0x7F;

// 2 banks of 8 encoders adressing 16 MPC Qlinks
static uint8_t KikpadEncodersBank = 0;
static uint8_t KikpadEncodersVal[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

// Kipad MPC control affectations
// 4 blocs returning all the same notes (but on a different midi channel)
static const uint8_t KikpadPadsNotesMap[] = {
  12,13,14,15,12,13,14,15,
   8, 9,10,11, 8, 9,10,11,
   4, 5, 6, 7, 4, 5, 6, 7,
   0, 1, 2, 3, 0, 1, 2, 3,
  12,13,14,15,12,13,14,15,
   8, 9,10,11, 8, 9,10,11,
   4, 5, 6, 7, 4, 5, 6, 7,
   0, 1, 2, 3, 0, 1, 2, 3,
};

// Corresponding colors
static const int8_t KikpadPadsColors[] = {
  GREEN,GREEN,GREEN,GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  GREEN,GREEN,GREEN,GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  GREEN,GREEN,GREEN,GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  GREEN,GREEN,GREEN,GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  RED  ,RED  ,RED  ,RED  ,BLUE,BLUE,BLUE,BLUE,
  RED  ,RED  ,RED  ,RED  ,BLUE,BLUE,BLUE,BLUE,
  RED  ,RED  ,RED  ,RED  ,BLUE,BLUE,BLUE,BLUE,
  RED  ,RED  ,RED  ,RED  ,BLUE,BLUE,BLUE,BLUE,
};

// MPC Buttons
// We need to define that to map them in the software
// Order defines the Note from 0 to N
enum MPCButtons {
  // Transport buttons
  PLAY,
  STOP,
  PLAY_START,
  STEP_BK,
  STEP_FWD,
  EVENT_BK,
  EVENT_FWD,
  RECORD,
  RECORD_FROM_SEQ_START,
  OVERDUB,
  PUNCH_IN,
  TAP_TEMPO,
  GLOBAL_AUTOMATION,
  TEMPO_SOURCE,
  MASTER_VOLUME,
  METRONOME_ENABLE,
  TIME_CORRECT,
  SWING,
  RETRO_RECORD,
  // Sequence parameters

} ;

// Map a KIKPAD button to a MPC button, in the same order
// as above.

static const uint8_t MPCButtonsMap[] = {
  // Transport buttons. 0xFF if unmapped
 BT_RIGHT,   // PLAY
 BT_DOWN,    // STOP
 BT_LEFT,    // PLAY_START
 0XFF,       // STEP_BK
 0XFF,       // STEP_FWD
 0XFF,       // EVENT_BK
 0XFF,       // EVENT_FWD
 0XFF,       // RECORD
 0XFF,       // RECORD_FROM_SEQ_START
 BT_UP,      // OVERDUB
 0XFF,       // PUNCH_IN
 BT_CONTROL4,// TAP_TEMPO
 0XFF,       // GLOBAL_AUTOMATION
 0XFF,       // TEMPO_SOURCE
 0XFF,       // MASTER_VOLUME
 0XFF,       // METRONOME_ENABLE
 0XFF,       // TIME_CORRECT
 0XFF,       // SWING
 0XFF,       // RETRO_RECORD
};

///////////////////////////////////////////////////////////////////////////////
// Get a MPC button value from a KikPad button
// -1 if not found
///////////////////////////////////////////////////////////////////////////////
static int16_t MPCButtonGetMap(uint8_t btnValue) {
  for ( uint8_t i = 0 ; i < sizeof(MPCButtonsMap) ; i++ ) {
      if ( MPCButtonsMap[i] == 0xFF ) continue;
      if ( btnValue == MPCButtonsMap[i] ) return i;
  }
  return -1;
}

///////////////////////////////////////////////////////////////////////////////
// Midi clocked actions
///////////////////////////////////////////////////////////////////////////////
static void MPCClockEvent() {

  if (MPCPlaying) {

    MPCClock++;
    MPCTick  = MPCClock % 24;
    MPCBeat  = (MPCClock / 24 ) %  4 ;
    MPCBar   = MPCClock / 24 / 4 ;

    // Show beat
    if ( MPCTick == 1 ) {
      ButtonSetLed(MPCButtonsMap[PLAY], ON);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
    }
    else if ( MPCTick == 6 ) {
      ButtonSetLed(MPCButtonsMap[PLAY], OFF);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], OFF);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], OFF);
    }

    // Show bars
    uint8_t b = MPCBar % 4;
    if ( b == 0 ) {
      ButtonSetLed(BT_MS4, OFF);
      ButtonSetLed(BT_MS1, ON);
    }
    else {
      ButtonSetLed(BT_MS1 + b - 1, OFF);
      ButtonSetLed(BT_MS1 + b, ON);
    }

  } else {
    // Clock received but not currently playing. Only show beat
    MPCTick  = ++MPCTick % 24;
    // Show beat
    if ( MPCTick == 1 ) {
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
    }
    else if ( MPCTick == 6 ) {
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], OFF);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{
  // Realtime
  if ( pk->packet[0] == 0x0F ) {

    switch (pk->packet[1]) {

      // Clock
      case 0xF8:
        MPCClockEvent();
        break;

      // Start
      case 0xFA:
        MPCPlaying = true;
        MPCClock = 0 ;
        ButtonSetLed(MPCButtonsMap[STOP], ON);

        break;

      // Continue
      case 0xFB:
        MPCPlaying = true;
        ButtonSetLed(MPCButtonsMap[STOP], ON);

        break;

      // Stop
      case 0xFC:
        MPCPlaying = false;
        ButtonSetLed(MPCButtonsMap[PLAY], ON);
        ButtonSetLed(MPCButtonsMap[STOP], OFF);
        ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
        ButtonSetLed(MPCButtonsMap[OVERDUB], ON);

        break;

      // Song pointer position
      case 0xF2:
        MPCClock = ((pk->packet[3] << 7) | pk->packet[2]) * 6;
        break;
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

  // midi packet. Static to keep last packet in memory.
  static midiPacket_t pk = { .i = 0 };

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise (turn right)
    case EV_EC_CW:
      {
        uint8_t i = idx + ( KikpadEncodersBank ? 8 : 0 ) ;
        if ( ++KikpadEncodersVal[i] > 127 ) KikpadEncodersVal[i] = 127;
        pk.packet[0] = 0x0B ;
        pk.packet[1] = 0xB0 + MPC_QLINKS_MIDI_CHANNEL ;
        pk.packet[2] = MPC_QLINKS_FIRST_CC_NUMBER + i ;
        pk.packet[3] = KikpadEncodersVal[i] ;
        MidiUSB.writePacket(&pk.i);
        break;
      }

    // Encoders Counter Clock wise (turn left)
    case EV_EC_CCW:
      {
        uint8_t i = idx + ( KikpadEncodersBank ? 8 : 0 ) ;
        if ( KikpadEncodersVal[i] > 0 ) KikpadEncodersVal[i]--;
        pk.packet[0] = 0x0B ;
        pk.packet[1] = 0xB0 + MPC_QLINKS_MIDI_CHANNEL ;
        pk.packet[2] = MPC_QLINKS_FIRST_CC_NUMBER + i ;
        pk.packet[3] = KikpadEncodersVal[i] ;
        MidiUSB.writePacket(&pk.i);
        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
    // Pad released
    case EV_PAD_RELEASED:
      {
        // Set the bank regarding the pad idx
        // 16 pads in the MPC.  4x16 in Kik pads
        uint8_t padBank = (idx % 8 > 3 ) ? 1 : 0 ;
        if ( idx < 32 ) padBank += 2;

        // Check if upper Kikpad bank
        if ( KikpadBank ) padBank += 4;

        // Send the note corresponding to the pressed pad
        if ( ev->ev == EV_PAD_PRESSED ) {
          PadSetColor(idx, WHITE);
          pk.packet[0] = 0x09;
          // Change the midi channel according to the pad bank
          pk.packet[1] = 0x90 + MPC_PADS_MIDI_CHANNEL + padBank ;
          pk.packet[3] = KikpadsVelocity;
        } else {
          PadSetColor(idx, KikpadPadsColors[idx]);
          pk.packet[0] = 0x08;
          // Change the midi channel according to the pad bank
          pk.packet[1] = 0x80 + MPC_PADS_MIDI_CHANNEL + padBank ;
          pk.packet[3] = 0x40;
        }

        pk.packet[2] = KikpadPadsNotesMap[idx];
        MidiUSB.writePacket(&pk.i);
      }

      break;

    // Button pressed and not released
    // Button released
    case EV_BTN_PRESSED:
    case EV_BTN_RELEASED:
      {
        // Is that button mapped to a MPC one ?
        int16_t value = MPCButtonGetMap(idx);
        if ( value  >= 0 ) {
          // Send Button midi message
          pk.packet[2] = value;
          pk.packet[3] = 0x40;

          if ( ev->ev == EV_BTN_PRESSED ) {
            // Note on - cable 0
            pk.packet[0] = 0x09 ;
            pk.packet[1] = 0x90 + MPC_BUTTON_MIDI_CHANNEL ;
          } else {
            // Note off
            pk.packet[0] = 0x08 ;
            pk.packet[1] = 0x80 + MPC_BUTTON_MIDI_CHANNEL ;

            // Specific button released cases
            switch (value) {
              // Toggle Overdub
              case OVERDUB:
                  MPCOverdub = !MPCOverdub;
                  ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
                  break;
            }
          }
          MidiUSB.writePacket(&pk.i);
        }
        else {
          // Kikpad Internal buttons

          // VOLUME. Encoders bank select.
          // Lock = SET + VOLUME
          if ( idx == BT_VOLUME ) {
            // 2 encoders banks 0-1. Light Volume if Bank 1 set.
            if ( ev->ev == EV_BTN_PRESSED  ) KikpadEncodersBank = 1;
            // Released : check if we must lock the mode
            else KikpadEncodersBank = ( ButtonIsPressed(BT_SET) ? 1:0 );

            ButtonSetLed(BT_VOLUME, KikpadEncodersBank);
          }

        }
        //ButtonSetLed(idx, ev->ev == EV_BTN_PRESSED ? ON:OFF );

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

  // Colors of pads
  memcpy(PadColorsCurrent,KikpadPadsColors,sizeof(KikpadPadsColors));
  RGBMaskUpdateAll();

  // Default ON buttons at start
  ButtonSetLed(MPCButtonsMap[PLAY_START], ON);
  ButtonSetLed(MPCButtonsMap[PLAY], ON);
  ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
  ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
static void KikpadMod_Loop() {

}

#endif
