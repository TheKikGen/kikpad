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


////////////////////////////////////////////////////////////////////////////////
// KIKPAD CLIP LAUNCHER FUNCTION KEYS
//------------------------------------------------------------------------------
// RESET                       = HOLD BT_CONTROL4 & MASTER8 THEN PRESS SET
// BOOTLOADER MODE             = HOLD BT_CONTROL4 & MASTER7 THEN PRESS SET
// CLIP + PAD PRESSED          = TOGGLE CLIP LOOP
// CLIP + SET                  = CLIP MODE NOTE ON / OFF . MIDI NOTE ON/OFF ONLY WHEN OFF
// CLIP + MODE1 HOLDED         = CLEAR ALL CLIPS
// MODE1 HOLDED + PAD PRESSED  = CLEAR CLIP

// VOLUME                      = TOGGLE LOWER/UPPER ENCODER BANK 1-8 / 8-16
//
// MASTER 1-8 + PAD PRESSED    = CHANGE CLIP LENGTH 1 - 8
////////////////////////////////////////////////////////////////////////////////

// KIKPAD_MPC : Advanced MPC midi controller

#ifndef _KIKPAD_MODULE_H_
#define _KIKPAD_MODULE_H_

#define ENCODERS_RELATIVE

#define MPC_PADS_MIDI_CHANNEL 0x00

#define MPC_CONTROL_MIDI_CHANNEL 0x0F

#define MPC_QLINKS_MIDI_CHANNEL MPC_CONTROL_MIDI_CHANNEL
#define MPC_BUTTON_MIDI_CHANNEL MPC_CONTROL_MIDI_CHANNEL

#define MPC_QLINKS_FIRST_CC_NUMBER 100

#define KIPAD_PAD_DEFAULT_VELOCITY 0x7F

#define KIKPAD_MAX_BAR_LEN 8

// Functions prototypes
static void KikpadClipStopAll() ;
static void KikpadClipSeqStateMachine();
static int16_t MPCButtonGetMap(uint8_t btnValue);
static void MPCClockEvent(void);
static void KikpadPadNoteOn(uint8_t idx,midiPacket_t *);
static void KikpadPadNoteOff(uint8_t idx,midiPacket_t *);
static void MPCButtonNoteOn(uint8_t value);
static void MPCButtonNoteOff(uint8_t value);
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx);
static void InitModesBarLeds();
static void KikpadClipsNoteOff();
static void KikpadClipsLedsOff();

// Globals

static boolean  MPCPlaying = false;
static boolean  MPCOverdub = false;

static uint16_t MPCBar   = 0;
static uint8_t  MPCBeat  = 0;
static uint16_t MPCClock = 0;
static uint8_t  MPCTick  = 0;


// True if clip mode note active
static boolean  KikpadModeClipNote = false ;

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
  LT_GREEN,LT_GREEN,LT_GREEN,LT_GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  LT_GREEN,LT_GREEN,LT_GREEN,LT_GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  LT_GREEN,LT_GREEN,LT_GREEN,LT_GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  LT_GREEN,LT_GREEN,LT_GREEN,LT_GREEN,MAGENTA,MAGENTA,MAGENTA,MAGENTA,
  LT_RED  ,LT_RED  ,LT_RED  ,LT_RED  ,BLUE,BLUE,BLUE,BLUE,
  LT_RED  ,LT_RED  ,LT_RED  ,LT_RED  ,BLUE,BLUE,BLUE,BLUE,
  LT_RED  ,LT_RED  ,LT_RED  ,LT_RED  ,BLUE,BLUE,BLUE,BLUE,
  LT_RED  ,LT_RED  ,LT_RED  ,LT_RED  ,BLUE,BLUE,BLUE,BLUE,
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

enum ClipStates {
  CLIP_EMPTY,
  CLIP_MUTED,
  CLIP_SYNC_MUTE,
  CLIP_SYNC_PLAY,
  CLIP_SYNC_REC,
};

enum ClipStatesColors {
  CLIP_COL_READY = WHITE,
  CLIP_COL_PLAY  = GREEN,
  CLIP_COL_RECORD = RED,
  CLIP_COL_PENDING = ORANGE,
};

// Pad clips
typedef struct{
  uint8_t state;
  uint8_t pos;
  uint8_t countBar;
  uint8_t lenBar;
  boolean loop;
  uint8_t bank;
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
// Show Clip len on the Master led bar
///////////////////////////////////////////////////////////////////////////////
static void ShowClipLen(uint len) {

  ButtonsBarSetLedMsk(BT_BAR_MASTER,0);
  for (uint8_t i = 0 ; i < len ; i ++ ) ButtonSetLed(BT_MS1 + i  , ON);

}

///////////////////////////////////////////////////////////////////////////////
// Show Current MPC Bar on the Master 1-8 buttons.
///////////////////////////////////////////////////////////////////////////////
static void ShowCurrentMPCBar() {

  // Preserve existing leds ON
  ButtonsBarSetLedMsk(BT_BAR_MASTER,0);
  ButtonSetLed(BT_MS1 + MPCBar  , ON);

}

///////////////////////////////////////////////////////////////////////////////
// Midi clocked actions
///////////////////////////////////////////////////////////////////////////////
static void MPCClockEvent() {

  MPCClock++;

  if (MPCPlaying) {

    if ( ++MPCTick == 1 ) {
      // Start a new bar
      if ( MPCBeat == 0 ) {
        KikpadClipSeqStateMachine();
        ShowCurrentMPCBar();
      }
      ButtonSetLed(MPCButtonsMap[PLAY], ON);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], ON);

    }
    else {
      if ( MPCTick == 6 ) {
        ButtonSetLed(MPCButtonsMap[PLAY], OFF);
        ButtonSetLed(MPCButtonsMap[TAP_TEMPO], OFF);
        if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], OFF);
      }
      else

      if ( MPCTick == 22  && MPCBeat == 3 ) KikpadClipsLedsOff();
      else

      if ( MPCTick == 24 ) {
          // Finish a current bar
          if ( MPCBeat == 3 ) KikpadClipsNoteOff();
          MPCTick = 0 ;
          if ( ++MPCBeat == 4 ) {        // Bar
              MPCBeat = 0 ;
              if ( ++MPCBar == KIKPAD_MAX_BAR_LEN ) MPCBar = 0;
          }
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Clear a Kikpad clip
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipClear(uint8_t idx) {

  KikpadClip[idx].state = CLIP_EMPTY ;
  KikpadClip[idx].pos = 0 ;
  KikpadClip[idx].lenBar = 0;
  KikpadClip[idx].countBar = 0 ;
  if ( KikpadClipMaxIdx && idx == KikpadClipMaxIdx ) KikpadClipMaxIdx-- ;
  if ( KikpadClipMinIdx != KikpadClipMaxIdx && idx == KikpadClipMinIdx ) KikpadClipMinIdx++ ;
  midiPacket_t pk;
  KikpadPadNoteOff(idx,&pk);
  PadSetColor(idx,KikpadPadsColors[idx]);

}

///////////////////////////////////////////////////////////////////////////////
// Launch a clip Row
///////////////////////////////////////////////////////////////////////////////
static void KikpadLaunchClipRow(uint8_t raw) {

  uint8_t start = raw/8 ;

  for ( uint8_t i = start  ; i < start + 8 ; i++ ) {
    if ( KikpadClip[i].state != CLIP_EMPTY ) {
      KikpadClip[i].state = CLIP_SYNC_PLAY;
      PadSetColor(i, CLIP_COL_PENDING);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Kikpad reset all clips
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipResetAll() {

  midiPacket_t pk;

  for ( uint8_t i ; i < PAD_SIZE ; i++ ) {
    KikpadClip[i].state = CLIP_EMPTY ;
    KikpadClip[i].pos = 0;
    KikpadClip[i].lenBar = 0;
    KikpadClip[i].countBar = 0 ;
    KikpadClip[i].loop = true;
    KikpadClip[i].bank =  ( (i % 8 > 3 ) ? 1 : 0 ) +  ( i < 32 ? 2: 0 );
    KikpadPadNoteOff(i,&pk);
  }
  KikpadClipMaxIdx = 0;
  KikpadClipMinIdx = PAD_SIZE ;
  memcpy(PadColorsCurrent,KikpadPadsColors,sizeof(KikpadPadsColors));
  RGBMaskUpdateAll();

}


///////////////////////////////////////////////////////////////////////////////
// Kikpad stop all clips but keep clip length
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipStartStopAll(boolean start) {

  midiPacket_t pk;

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
    if ( KikpadClip[i].state != CLIP_EMPTY ) {
      if ( KikpadClip[i].state == CLIP_SYNC_REC ) {
          if ( !start) KikpadClipClear(i);
      }
      else if ( KikpadClip[i].state == CLIP_SYNC_MUTE ) {
          if ( !start) KikpadPadNoteOff(i,&pk);
          KikpadClip[i].state == CLIP_MUTED ;
          KikpadClip[i].countBar = 0 ;
          PadSetColor(i, KikpadPadsColors[i]);
      }
      else if ( KikpadClip[i].state == CLIP_SYNC_PLAY ) {
          if ( !start) KikpadPadNoteOff(i,&pk);
          else KikpadClip[i].countBar = 0  ;
          PadSetColor(i, CLIP_COL_PENDING);
      }
    }
  }

}

///////////////////////////////////////////////////////////////////////////////
// Change Len of a clip. Return len if done.
///////////////////////////////////////////////////////////////////////////////
static uint8_t KikpadClipChangeLen(uint8_t idx) {

  // Change length if MS-1 MS-4 pressed
  for ( uint8_t i = 0 ; i < KIKPAD_MAX_BAR_LEN ; i++ ) {
    if ( ButtonIsPressed( BT_MS1 + i ) ) {
      PadSetColor(idx, CLIP_COL_PENDING);
      KikpadClip[idx].lenBar = i + 1 ;
      if (KikpadClip[i].countBar > KikpadClip[idx].lenBar) KikpadClip[i].countBar = 0 ;
      KikpadClip[idx].state ==  CLIP_SYNC_PLAY  ;
      ShowClipLen(KikpadClip[idx].lenBar) ;
      return KikpadClip[idx].lenBar;
    }
  }

  return  0;

}

///////////////////////////////////////////////////////////////////////////////
// Send Clips notes off at the end of the right bar Tick 24 / Beat 3
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipsNoteOff() {
  // Last tick/beat before a new bar : we need to send clip note off here
  // to avoid notes overlap at the beggining of a new bar
  // Mute doesn't wait end of clip

  midiPacket_t pk;

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
        if ( KikpadClip[i].state == CLIP_SYNC_MUTE
            || ( KikpadClip[i].countBar == 0 && KikpadClip[i].state == CLIP_SYNC_PLAY ) ) {
                KikpadPadNoteOff(i,&pk);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Clips led off at the end of the right bar Tick 23 / Beat 4
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipsLedsOff() {
  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
        if ( KikpadClip[i].state == CLIP_SYNC_MUTE
            || ( KikpadClip[i].countBar == 0 && KikpadClip[i].state == CLIP_SYNC_PLAY ) ) {
                PadSetColor(i, KikpadPadsColors[i]);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Launch and loop clip - Clip sequencer state machine
// ----------------------------------------------------------------------------
// STATE DIAGRAM :
//
//  CLIP_EMPTY -> CLIP_SYNC_REC -> CLIP_SYNC_PLAY <-> CLIP_MUTED
//
//
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipSeqStateMachine() {

  midiPacket_t pk;

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {

    switch (KikpadClip[i].state) {

      case CLIP_EMPTY:
      case CLIP_MUTED:
          break;

      case CLIP_SYNC_PLAY:
          if (  KikpadClip[i].countBar == 0 )  {
            PadSetColor(i, CLIP_COL_PLAY );
            KikpadPadNoteOn(i, &pk );
          }
          if ( ++KikpadClip[i].countBar == KikpadClip[i].lenBar) KikpadClip[i].countBar = 0 ;
          break;

      case CLIP_SYNC_MUTE:
          KikpadClip[i].countBar = 0 ;
          KikpadClip[i].state = CLIP_MUTED ;
          PadSetColor(i, KikpadPadsColors[i]);
          break;

      case CLIP_SYNC_REC:
          if ( KikpadClip[i].lenBar++ == 0 ) {
            KikpadClip[i].pos = MPCBar ;
            PadSetColor(i, CLIP_COL_RECORD);
            KikpadPadNoteOn(i,&pk);
          }
          else if ( KikpadClip[i].lenBar == KIKPAD_MAX_BAR_LEN ) {
            // Max length. Stop recording
            PadSetColor(i, CLIP_COL_PENDING);
            KikpadClip[i].state = CLIP_SYNC_PLAY ;
          }
          break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// CLIP MODE EVENT STATE MACHINE
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx) {

  if ( ev == EV_PAD_PRESSED ) {

    if ( KikpadClip[idx].state == CLIP_EMPTY ) {

      // Record clip
      KikpadClip[idx].state = CLIP_SYNC_REC ;
      PadSetColor(idx, CLIP_COL_PENDING);

      // seq Loop lazy optimization
      if ( idx > KikpadClipMaxIdx ) KikpadClipMaxIdx = idx ;
      if ( idx < KikpadClipMinIdx)  KikpadClipMinIdx = idx ;

    }
    // Clip not empty
    else {

      // Specific clip actions

      // Clear a clip if MODE1 Holded
      // Will send a note off immediatly
      if ( ButtonIsHolded(BT_MODE1) ) {
          KikpadClipClear(idx);
          return;
      }

      // Change length eventually if MS-1 MS-4 pressed
      // That will reset position at the next bar
      if ( KikpadClipChangeLen(idx) ) return ;

      // Start a clip if stopped
      if ( KikpadClip[idx].state == CLIP_MUTED ) {
        KikpadClip[idx].state = CLIP_SYNC_PLAY;
        PadSetColor(idx, CLIP_COL_PENDING);
      }
      else

      // Stop a clip if playing
      if ( KikpadClip[idx].state ==  CLIP_SYNC_PLAY ) {
        if ( MPCPlaying ) {
            KikpadClip[idx].state = CLIP_SYNC_MUTE;
            PadSetColor(idx, CLIP_COL_PENDING);
        }
        else {
          // Deactivate sync play
          KikpadClip[idx].state = CLIP_MUTED;
          PadSetColor(idx, KikpadPadsColors[idx]);
        }
      }
    }
  }
  else
  if ( ev == EV_PAD_RELEASED ){
    // PAD RELEASED
    switch (KikpadClip[idx].state) {

      case CLIP_SYNC_REC:
        // Create a clip with a fixed length eventually if MS-1 MS-4 pressed
        KikpadClipChangeLen(idx);

        // Check if record stopped before the next bar...
        if ( KikpadClip[idx].lenBar == 0 ) KikpadClip[idx].lenBar = 1;
        KikpadClip[idx].state = CLIP_SYNC_PLAY;
        break;
    }

  }

}

///////////////////////////////////////////////////////////////////////////////
// INITIALIZE BOTTOM MODES BUTTON BAR LEDS
///////////////////////////////////////////////////////////////////////////////
static void InitModesBarLeds() {
  ButtonSetLed(MPCButtonsMap[PLAY_START], ON);
  ButtonSetLed(MPCButtonsMap[PLAY], ON);
  ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
  ButtonSetLed(MPCButtonsMap[OVERDUB], ON);
  ButtonSetLed(BT_CLIP,KikpadModeClipNote ? OFF:ON);
}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USB MIDI PACKET
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_USBMidiParse(midiPacket_t *pk)
{
  // Realtime on cable 0 only
  if ( pk->packet[0] == 0x0F ) {

    switch (pk->packet[1]) {

      // Clock
      case 0xF8:
        MPCClockEvent();
        break;

      // Start
      case 0xFA:
        MPCPlaying = true;
        ButtonSetLed(MPCButtonsMap[STOP], ON);
        KikpadClipStartStopAll(true);
        MPCClock = MPCBar = MPCBeat = MPCTick = 0;

        break;

      // Continue
      case 0xFB:
        MPCPlaying = true;
        ButtonSetLed(MPCButtonsMap[STOP], ON);

        break;

      // Stop
      case 0xFC:
        MPCPlaying = false;
        InitModesBarLeds();
        KikpadClipStartStopAll(false);
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
// SEND A MPC Mapped Button Note ON on cable 0
///////////////////////////////////////////////////////////////////////////////
static void MPCButtonNoteOn(uint8_t value) {

  midiPacket_t pk = { .i = 0 };

  // Note on - cable 0
  pk.packet[0] = 0x09 ;
  pk.packet[1] = 0x90 + MPC_BUTTON_MIDI_CHANNEL ;
  // Send Button midi message
  pk.packet[2] = value ;
  pk.packet[3] = 0x40;

  MidiUSB.writePacket(&pk.i);

}

///////////////////////////////////////////////////////////////////////////////
// SEND A MPC Mapped Button Note OFF on cable 0
///////////////////////////////////////////////////////////////////////////////
static void MPCButtonNoteOff(uint8_t value) {

  midiPacket_t pk = { .i = 0 };

  // Note off
  pk.packet[0] = 0x08 ;
  pk.packet[1] = 0x80 + MPC_BUTTON_MIDI_CHANNEL ;
  // Send Button midi message
  pk.packet[2] = value ;
  pk.packet[3] = 0x40;

  MidiUSB.writePacket(&pk.i);

}

///////////////////////////////////////////////////////////////////////////////
// SEND PAD NOTE ON / OFF
///////////////////////////////////////////////////////////////////////////////
static void KikpadPadNoteOn(uint8_t idx,midiPacket_t *pk) {
  pk->i = 0;

  // Send the note corresponding to the pressed pad
  // NB : Due to the multi-channel note off issue, we change port (cable), and not
  // midi channel anymore...

  pk->packet[0] = 0x09 +  (  ( MPC_PADS_MIDI_CHANNEL + KikpadClip[idx].bank +  (KikpadBank ? 4:0) ) << 4 );
  pk->packet[1] = 0x90 ; // + MPC_PADS_MIDI_CHANNEL + KikpadClip[idx].bank +  (KikpadBank ? 4:0) ;
  pk->packet[2] = KikpadPadsNotesMap[idx] ;
  pk->packet[3] = KikpadPadVelocity;
  MidiUSB.writePacket(&pk->i);
}


///////////////////////////////////////////////////////////////////////////////
// SEND PAD NOTE OFF
///////////////////////////////////////////////////////////////////////////////
static void KikpadPadNoteOff(uint8_t idx,midiPacket_t *pk) {

  pk->i = 0;

  // Send the note corresponding to the pressed pad
  // NB : Due to the multi-channel note off issue, we change port (cable), and not
  // midi channel anymore...

  pk->packet[0] = 0x08 + ( ( MPC_PADS_MIDI_CHANNEL + KikpadClip[idx].bank +  (KikpadBank ? 4:0) ) << 4 );
  pk->packet[1] = 0x80 ; //+ MPC_PADS_MIDI_CHANNEL + + KikpadClip[idx].bank +  (KikpadBank ? 4:0) ;
  pk->packet[2] = KikpadPadsNotesMap[idx] ;
  pk->packet[3] = 0x40;
  MidiUSB.writePacket(&pk->i);
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
        // Compute accelerator factor regarding the turn speed.
        uint8_t accel = 50 * 1000 / eventDelayMicros ;

        #if defined(ENCODERS_RELATIVE)

          if ( ev->ev == EV_EC_CCW )
            KikpadEncodersVal[e] = 0x7f - accel ;
          else
            KikpadEncodersVal[e] = 1 + accel ;

        #else
          // Compute accelerator factor if <  100 millisecs
          KikpadEncodersVal[e] += ( eventDelayMicros < 100*1000 ? 6:1 ) * ( ev->ev == EV_EC_CW ? 1 : -1);

          // as KikpadEncodersVal is unsigned, > 127 means also negative
          if ( KikpadEncodersVal[e] > 127 ) KikpadEncodersVal[e] = ( ev->ev == EV_EC_CW ? 127 : 0 );

        #endif

        MPCEncoderSendCC(e,KikpadEncodersVal[e]);
        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
        if (! KikpadModeClipNote ) {
          KikpadClipEvStateMachine(ev->ev,idx);
        }
        else { // standard Midi mode
          midiPacket_t pk;
          PadSetColor(idx, WHITE );
          KikpadPadNoteOn(idx,&pk);
        }
        break;

    // Pad released
    case EV_PAD_RELEASED:
        if ( !KikpadModeClipNote )
          KikpadClipEvStateMachine(ev->ev,idx);
        else {
          midiPacket_t pk;
          PadSetColor(idx, KikpadPadsColors[idx]);
          KikpadPadNoteOff(idx,&pk);
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
            MPCButtonNoteOn(value);
          }
          else {
            // Note off
            MPCButtonNoteOff(value);

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

          // VOLUME. Toggle encoders bank.
          // 2 encoders banks 0-1. Light Volume if Bank 1 set.
          if ( idx == BT_VOLUME && ev->ev == EV_BTN_PRESSED ) {
              KikpadEncodersBank = ! KikpadEncodersBank ;
              ButtonSetLed(BT_VOLUME, KikpadEncodersBank);
          }
          else

          if ( idx == BT_SET && ev->ev == EV_BTN_PRESSED ) {

              // Toggle Clip mode. CLIP + SET
              if ( ButtonIsPressed(BT_CLIP) )  {
                KikpadModeClipNote = ! KikpadModeClipNote;
                ButtonSetLed(BT_CLIP,KikpadModeClipNote ? OFF:ON);
              }

          }
        }
        //ButtonSetLed(idx, ev->ev == EV_BTN_PRESSED ? ON:OFF );

      }

      break;

    // Button pressed and holded more than 2s
    case EV_BTN_HOLDED:

      // CLIP + HOLD MODE 1 = Clear all clips in clip mode -
      if ( idx == BT_MODE1 && ButtonIsPressed(BT_CLIP) ) {
          KikpadClipResetAll();
      }

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


  // Reset all clips
  KikpadClipResetAll();

  // Default ON buttons at start
  InitModesBarLeds();


}
///////////////////////////////////////////////////////////////////////////////
// Kikpad module loop
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_Loop() {

}

#endif
