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

#define KIPAD_PAD_DEFAULT_VELOCITY 0x7F

// Functions prototypes
static void KikpadClipStopAll() ;
static void KikpadClipSeqStateMachine();
static int16_t MPCButtonGetMap(uint8_t btnValue);
static void MPCClockEvent(void);
static void KikpadPadSendNote(uint8_t idx,boolean noteOn,uint8_t velocity = KIPAD_PAD_DEFAULT_VELOCITY);
static void MPCButtonSendNote(uint8_t value, boolean noteOn);
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx);

// Globals

static boolean  MPCPlaying = false;
static boolean  MPCOverdub = false;
static uint16_t MPCBar   = 0;
static uint8_t  MPCBeat  = 0;
static uint16_t MPCClock = 0;
static uint8_t  MPCTick  = 0;


static boolean  KikpadModeClip = true;
// Max Clip bar
static uint8_t KikpadClipMaxBarLen = 4;


// 2 banks of 64 pads for Kikpad
static uint8_t KikpadBank = 0;

// 8 banks A-H of 16 MPC pads
static uint8_t MPCPadsBank = 0;
static const uint8_t KikpadPadsBankNotesMap[]={16,17,18,19,20,21,22,23};

// Current pads velocity
static uint8_t KikpadPadVelocity = KIPAD_PAD_DEFAULT_VELOCITY;

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

enum ClipStatus {
  CLIP_EMPTY,
  CLIP_STOPPED,
  CLIP_END_PLAYING,
  CLIP_PLAYING,
  CLIP_RECORDING,
};

// Pad clips
typedef struct{
  uint8_t state;
  uint8_t startBar;
  uint8_t lenBar;
}  __packed KikpadClip_t;

KikpadClip_t KikpadClip[PAD_SIZE];
uint8_t KikpadClipMaxIdx = 0 ;
uint8_t KikpadClipMinIdx = PAD_SIZE ;

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
      if ( KikpadModeClip ) KikpadClipSeqStateMachine();

    }
    else if ( MPCTick == 6 ) {
      ButtonSetLed(MPCButtonsMap[PLAY], OFF);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], OFF);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], OFF);
    }

    if ( MPCBeat == 0 ) {
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
    }

  }
}

///////////////////////////////////////////////////////////////////////////////
// Kikpad reset all clips
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipResetAll() {

  for ( uint8_t i ; i < PAD_SIZE ; i++ ) {
    KikpadClip[i].state = CLIP_EMPTY ;
    KikpadClip[i].startBar = 0;
    KikpadClip[i].lenBar = 0;
  }
  KikpadClipMaxIdx = 0;
  KikpadClipMinIdx = PAD_SIZE ;
}

///////////////////////////////////////////////////////////////////////////////
// Kikpad stop all clips
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipStopAll() {

  for (uint8_t i = 0 ; i < PAD_SIZE ; i++ ) {
    KikpadPadSendNote(i,false); // Note OFF
    KikpadClip[i].state = CLIP_STOPPED;
  }
  memcpy(PadColorsCurrent,KikpadPadsColors,sizeof(KikpadPadsColors));
  RGBMaskUpdateAll();
  KikpadClipMaxIdx = 0 ;
  KikpadClipMinIdx = PAD_SIZE ;

}
///////////////////////////////////////////////////////////////////////////////
// Clocked events Clip sequencer state machine
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipSeqStateMachine() {

  uint8_t bar = MPCBar % KikpadClipMaxBarLen;

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
  //for ( uint8_t i = 0 ; i < PAD_SIZE ; i++ ) {

    switch (KikpadClip[i].state) {

      case CLIP_PLAYING:
          // Beginning of a bar. Check if we must launch/loop the clip
          if ( MPCBeat == 0 ) {
              if ( bar == KikpadClip[i].startBar  % KikpadClipMaxBarLen ) {
                PadSetColor(i, CYAN );
                // Update next bar
                KikpadClip[i].startBar = ( KikpadClip[i].startBar + KikpadClip[i].lenBar ) % KikpadClipMaxBarLen ;
                KikpadPadSendNote(i,true,KikpadPadVelocity );
              }
          }
          else if ( MPCBeat == 2 ) PadSetColor(i, KikpadPadsColors[i]);

          break;

      case CLIP_END_PLAYING:
          if ( MPCBeat == 0 ) {
            PadSetColor(i, KikpadPadsColors[i]);
            KikpadPadSendNote(i,false); // Note OFF
            KikpadClip[i].state = CLIP_STOPPED ;
          }
          break;

      case CLIP_RECORDING:
          if ( MPCBeat == 0 ) {
            if ( KikpadClip[i].lenBar == 0 ) {
              if ( bar == KikpadClip[i].startBar % KikpadClipMaxBarLen ) {
                  KikpadPadSendNote(i,true,KikpadPadVelocity); // Note ON
                  PadSetColor(i, YELLOW);
                  KikpadClip[i].lenBar++  ;
              }
            } else
            if ( KikpadClip[i].lenBar < KikpadClipMaxBarLen ) KikpadClip[i].lenBar++;
            else  KikpadClip[i].state = CLIP_PLAYING ;
          }
          //else if ( MPCBeat == 2 ) PadSetColor(i, KikpadPadsColors[i]);
          break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// CLIP MODE EVENT STATE MACHINE
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx) {

  if ( ev == EV_PAD_PRESSED ) {

    PadSetColor(idx, WHITE);

    switch (KikpadClip[idx].state) {

      case CLIP_EMPTY:

        // seq Loop lazy optimization
        KikpadClipMaxIdx = ( idx > KikpadClipMaxIdx ? idx : KikpadClipMaxIdx );
        KikpadClipMinIdx = ( idx < KikpadClipMinIdx ? idx : KikpadClipMinIdx );

        // If the clip is empty, start to record len
        // Save the next  bar

        // Automatically start...
        if ( ! MPCPlaying ) {
           MPCButtonSendNote(PLAY_START,true);
           MPCButtonSendNote(PLAY_START,false);
           KikpadClip[idx].startBar = 255 ; // next increment will be 0 because of uint8
        }
        else KikpadClip[idx].startBar = ( MPCBar + 1 )  % KikpadClipMaxBarLen ;

        KikpadClip[idx].state = CLIP_RECORDING ;
        break;

      case CLIP_STOPPED:
        // Pad pressed = start clip
        KikpadClip[idx].startBar =  ( MPCBar + 1 )  % KikpadClipMaxBarLen ;
        KikpadClip[idx].state = CLIP_PLAYING;
        break;

      case CLIP_PLAYING:
        KikpadClip[idx].state = CLIP_END_PLAYING;
        break;
    }
  }
  else if ( ev == EV_PAD_RELEASED ){
    // PAD RELEASED
    switch (KikpadClip[idx].state) {

      case CLIP_RECORDING:
        // Check if record stopped before the next bar...
        if ( KikpadClip[idx].lenBar == 0 ) KikpadClip[idx].lenBar = 1;
        KikpadClip[idx].state = CLIP_PLAYING;
        break;
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
        if ( KikpadModeClip ) KikpadClipStopAll();
        // DEBUG
          KikpadClipResetAll();

        break;

      // Song pointer position
      case 0xF2:
        MPCClock = ((pk->packet[3] << 7) | pk->packet[2]) * 6;
        break;
    }
  }
}



///////////////////////////////////////////////////////////////////////////////
// SEND a MPC mapped ENCODER CC midi message
///////////////////////////////////////////////////////////////////////////////
static void MPCEncoderSendCC(uint8_t encoder,uint8_t ccValue) {

  midiPacket_t pk = { .i = 0 };

  pk.packet[0] = 0x0B ;
  pk.packet[1] = 0xB0 + MPC_QLINKS_MIDI_CHANNEL ;
  pk.packet[2] = MPC_QLINKS_FIRST_CC_NUMBER + encoder ;
  pk.packet[3] = ccValue ;
  MidiUSB.writePacket(&pk.i);

}

///////////////////////////////////////////////////////////////////////////////
// SEND A MPC Mapped Button Note ON/OFF
///////////////////////////////////////////////////////////////////////////////
static void MPCButtonSendNote(uint8_t value, boolean noteOn) {

  midiPacket_t pk = { .i = 0 };

  // Send Button midi message
  pk.packet[2] = value;
  pk.packet[3] = 0x40;

  if ( noteOn ) {
    // Note on - cable 0
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x90 + MPC_BUTTON_MIDI_CHANNEL ;
  } else {
    // Note off
    pk.packet[0] = 0x08 ;
    pk.packet[1] = 0x80 + MPC_BUTTON_MIDI_CHANNEL ;
    pk.packet[3] = 0x00;
  }

  MidiUSB.writePacket(&pk.i);

}

///////////////////////////////////////////////////////////////////////////////
// SEND PAD NOTE ON / OFF
///////////////////////////////////////////////////////////////////////////////
static void KikpadPadSendNote(uint8_t idx,boolean noteOn,uint8_t velocity) {

  midiPacket_t pk = { .i = 0 };

  // Set the bank regarding the pad idx
  // 16 pads in the MPC.  4x16 in Kik pads
  uint8_t padBank = (idx % 8 > 3 ) ? 1 : 0 ;
  if ( idx < 32 ) padBank += 2;

  // Check if upper Kikpad bank
  if ( KikpadBank ) padBank += 4;


  // Send the note corresponding to the pressed pad
  if ( noteOn ) {
    pk.packet[0] = 0x09;
    // Change the midi channel according to the pad bank
    pk.packet[1] = 0x90 + MPC_PADS_MIDI_CHANNEL + padBank ;
    pk.packet[3] = velocity;
  }
  else { // Note Off
    pk.packet[0] = 0x08;
    // Change the midi channel according to the pad bank
    pk.packet[1] = 0x80 + MPC_PADS_MIDI_CHANNEL + padBank ;
    pk.packet[3] = 0x00;
  }

  pk.packet[2] = KikpadPadsNotesMap[idx];
  MidiUSB.writePacket(&pk.i);
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

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise (turn right)
    case EV_EC_CW:
    // Encoders Counter Clock wise (turn left)
    case EV_EC_CCW:

      {
        uint8_t e = idx + ( KikpadEncodersBank ? 8 : 0 ) ;
        // Compute accelerator factor if <  100 millisecs
        KikpadEncodersVal[e] += ( eventDelayMicros < 100*1000 ? 6:1 ) * ( ev->ev == EV_EC_CW ? 1 : -1);
        // as KikpadEncodersVal is unsigned, > 127 means also negative
        if ( KikpadEncodersVal[e] > 127 ) KikpadEncodersVal[e] = ( ev->ev == EV_EC_CW ? 127 : 0 );

        MPCEncoderSendCC(e,KikpadEncodersVal[e]);
        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
        if ( KikpadModeClip ) {
          KikpadClipEvStateMachine(ev->ev,idx);
        }
        else { // standard Midi mode
          PadSetColor(idx, WHITE );
          KikpadPadSendNote(idx,true,KikpadPadVelocity);
        }
        break;

    // Pad released
    case EV_PAD_RELEASED:
        if ( KikpadModeClip ) KikpadClipEvStateMachine(ev->ev,idx);
        else {
          PadSetColor(idx, KikpadPadsColors[idx]);
          KikpadPadSendNote(idx,false);
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
          if ( ev->ev == EV_BTN_PRESSED ) {
            MPCButtonSendNote(value,true);
          }
          else {
            // Note off
            MPCButtonSendNote(value,false);

            // Specific buttons released cases
            switch (value) {
              // Toggle Overdub
              case OVERDUB:
                  MPCOverdub = !MPCOverdub;
                  ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
                  break;
            }
          }
        }
        else {
          // Kikpad Internal buttons not mapped

          // VOLUME. Encoders bank select.
          // Lock = SET + VOLUME
          if ( idx == BT_VOLUME ) {
            // 2 encoders banks 0-1. Light Volume if Bank 1 set.
            if ( ev->ev == EV_BTN_PRESSED  ) KikpadEncodersBank = 1;
            // Released : check if we must lock the mode
            else KikpadEncodersBank = ( ButtonIsPressed(BT_SET) ? 1:0 );

            ButtonSetLed(BT_VOLUME, KikpadEncodersBank);
          }
          else
          // Toggle Clip mode
          if ( idx == BT_CLIP && ev->ev == EV_BTN_RELEASED ) {
              KikpadModeClip = ! KikpadModeClip;
              ButtonSetLed(BT_CLIP,KikpadModeClip ? ON:OFF);
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
void KikpadMod_Setup() {

  // Colors of pads
  memcpy(PadColorsCurrent,KikpadPadsColors,sizeof(KikpadPadsColors));
  RGBMaskUpdateAll();

  // Default ON buttons at start
  ButtonSetLed(MPCButtonsMap[PLAY_START], ON);
  ButtonSetLed(MPCButtonsMap[PLAY], ON);
  ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
  ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
  ButtonSetLed(BT_CLIP,KikpadModeClip ? ON:OFF);

  // Reset all clips
  KikpadClipResetAll();


}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_Loop() {

}

#endif
