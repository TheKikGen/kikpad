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
//                             = Can be used to play the pad in real time

// MODE1 = ERASE KEY
// MODE1 HOLDED + CLIP         = CLEAR ALL CLIPS
// MODE1 HOLDED + PAD PRESSED  = CLEAR CLIP ON PAD



// VOLUME                      = TOGGLE LOWER/UPPER ENCODER BANK 1-8 / 8-16
// SENDA + PAD                 = Send PAD PAD NOTE on channel 16 when mapped a global
//                               Can be used to select the pad and
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

// Check globally if one or many pad currently pressed
static boolean PadPressed = false;

// Check globally if one or many button are currently pressed
static boolean ButtonPressed = false;


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
  NEXT_TRACK,
  PREVIOUS_TRACK,
  FIRST_TRACK,
  LAST_TRACK,
  NEXT_QLINK_MODE,
  PREVIOUS_QLINK_MODE,
  FIRST_QLINK_MODE,
  LAST_QLINK_MODE,

  // End
  MPC_BUTTON_MAX
} ;

enum MPCButtonsType {
  MAP_TO_NOTE,
  MAP_TO_CC,
};

// Map a KIKPAD button to a MPC button, in the same order
// as above.
// Pad clips
typedef struct{
  uint8_t btnValue;
  uint8_t type;
  uint8_t value1;
  uint8_t value2;
}  __packed MPCButtonsMap_t;


static const MPCButtonsMap_t MPCButtonsMap[] = {
  // Transport buttons. 0xFF if unmapped
 { BT_RIGHT,   MAP_TO_NOTE, 0x00, 0x7F },       // PLAY
 { BT_DOWN,    MAP_TO_NOTE, 0x01, 0x7F },       // STOP
 { BT_LEFT,    MAP_TO_NOTE, 0x02, 0x7F },       // PLAY_START
 { 0XFF,       MAP_TO_NOTE, 0x03, 0x7F },       // STEP_BK
 { 0XFF,       MAP_TO_NOTE, 0x04, 0x7F },       // STEP_FWD
 { 0XFF,       MAP_TO_NOTE, 0x05, 0x7F },       // EVENT_BK
 { 0XFF,       MAP_TO_NOTE, 0x06, 0x7F },       // EVENT_FWD
 { 0XFF,       MAP_TO_NOTE, 0x07, 0x7F },       // RECORD
 { 0XFF,       MAP_TO_NOTE, 0x08, 0x7F },       // RECORD_FROM_SEQ_START
 { BT_UP,      MAP_TO_NOTE, 0x09, 0x7F },       // OVERDUB
 { 0XFF,       MAP_TO_NOTE, 0x0A, 0x7F },       // PUNCH_IN
 { 0XFF,       MAP_TO_NOTE, 0x0B, 0x7F },       // TAP_TEMPO
 { 0XFF,       MAP_TO_NOTE, 0x0C, 0x7F },       // GLOBAL_AUTOMATION
 { 0XFF,       MAP_TO_NOTE, 0x0D, 0x7F },       // TEMPO_SOURCE
 { 0XFF,       MAP_TO_NOTE, 0x0E, 0x7F },       // MASTER_VOLUME
 { 0XFF,       MAP_TO_NOTE, 0x0F, 0x7F },       // METRONOME_ENABLE
 { 0XFF,       MAP_TO_NOTE, 0x10, 0x7F },       // TIME_CORRECT
 { 0XFF,       MAP_TO_NOTE, 0x11, 0x7F },       // SWING
 { 0XFF,       MAP_TO_NOTE, 0x12, 0x7F },       // RETRO_RECORD

 { 0XFF,       MAP_TO_CC,   0x01, 0x01 },       // NEXT_TRACK
 { 0XFF,       MAP_TO_CC,   0x01, 0x7F },       // PREVIOUS_TRACK
 { 0XFF,       MAP_TO_CC,   0x01, 0x7F - 0x30 },       // FIRST_TRACK
 { 0XFF,       MAP_TO_CC,   0x01, 0x7F + 0X30 },       // LAST_TRACK

 { 0XFF,       MAP_TO_CC,   0x02, 0x01 },       // NEXT_QLINK_MODE
 { 0XFF,       MAP_TO_CC,   0x02, 0x7F },       // PREVIOUS_QLINK_MODE
 { 0XFF,       MAP_TO_CC,   0x02, 0x7F - 0x30 },// FIRST_QLINK_MODE
 { 0XFF,       MAP_TO_CC,   0x02, 0x7F + 0X30 },// LAST_QLINK_MODE

};

enum ClipStates {
  CLIP_EMPTY,
  CLIP_MUTED,
  CLIP_SYNC_MUTE,
  CLIP_SYNC_LAUNCH,
  CLIP_SYNC_ONESHOT,
  CLIP_PLAYING,
  CLIP_SYNC_REC,
};

enum SeqStates {
  SEQ_NEW_BAR,
  SEQ_PAD_PULSE,
  SEQ_LAST_TICK_IN_BAR,
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

// Functions prototypes ///////////////////////////////////////////////////////

static int16_t MPCButtonGetMap(uint8_t btnValue);
static void ShowClipLen(uint len);
static void ShowCurrentMPCBar();
static void MPCClockEvent();
static void KikpadClipClear(uint8_t idx);
static void KikpadLaunchClipRow(uint8_t row,boolean launch);
static void KikpadClearClipRow(uint8_t row);
static void KikpadClipResetAll();
static void KikpadClipStartStopAll(boolean start);
static uint8_t KikpadClipChangeLen(uint8_t idx);
static void KikpadClipSequencer(SeqStates state);
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx);
static void InitModesBarLeds();
void KikpadMod_USBMidiParse(midiPacket_t *pk);
static void MPCEncoderSendCC(uint8_t encoder,uint8_t ccValue);
static void MPCButtonNoteOn(uint8_t value);
static void MPCButtonNoteOff(uint8_t value);
static void KikpadPadNoteOn(uint8_t idx,midiPacket_t *pk);
static void KikpadPadNoteOff(uint8_t idx,midiPacket_t *pk);
static void MPCGotoTrack(uint8_t track);
static void MPCPadSceneTrack(uint8_t track);

void KikpadMod_ProcessUserEvent(UserEvent_t *ev);

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Get a MPC button value from a KikPad button
// -1 if not found
///////////////////////////////////////////////////////////////////////////////
static int16_t MPCButtonGetMap(uint8_t btnValue) {
  for ( uint8_t i = 0 ; i < MPC_BUTTON_MAX ; i++ ) {
      if ( MPCButtonsMap[i].btnValue == 0xFF ) continue;
      if ( btnValue == MPCButtonsMap[i].btnValue ) return i;
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

  ButtonsBarSetLedMsk(BT_BAR_MASTER,0);
  ButtonSetLed(BT_MS1 + MPCBar  , ON);

}

///////////////////////////////////////////////////////////////////////////////
// CLIP MODE EVENT STATE MACHINE
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipEvStateMachine(uint8_t ev,uint8_t idx) {

  if ( ev == EV_PAD_PRESSED ) {

    // Pad scene mode
    if ( ButtonIsPressed(BT_SENDA) ) {

      // Goto Track
      MPCPadSceneTrack(KikpadClip[idx].bank);
      return;
    }


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
      if ( ButtonIsHolded(BT_MODE1) ) { KikpadClipClear(idx); return; }


      // Toggle Clip Loop
      if ( ButtonIsPressed(BT_CLIP) ) {
          KikpadClip[idx].loop = !KikpadClip[idx].loop ;
          PadSetColor(idx, CLIP_COL_PENDING);
          return;
      }

      // Change length eventually if MS-1 MS-4 pressed
      // That will reset position at the next bar
      if ( KikpadClipChangeLen(idx) ) return ;



      // Start a clip if stopped
      if ( KikpadClip[idx].state == CLIP_MUTED ) {
        KikpadClip[idx].state = CLIP_SYNC_LAUNCH;
        PadSetColor(idx, CLIP_COL_PENDING);
      }
      else

      // Stop a clip if playing
      if ( KikpadClip[idx].state == CLIP_PLAYING      ||
           KikpadClip[idx].state == CLIP_SYNC_ONESHOT ||
           KikpadClip[idx].state == CLIP_SYNC_LAUNCH ) {

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
        KikpadClip[idx].state = CLIP_SYNC_LAUNCH;
        break;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Launch and loop clip - Clip sequencer state machine
// ----------------------------------------------------------------------------
// STATE DIAGRAM :
//
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipSequencer(SeqStates state) {

  midiPacket_t pk;

  if ( state == SEQ_NEW_BAR ) {

    ShowCurrentMPCBar();

    for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {

      switch (KikpadClip[i].state) {

        case CLIP_EMPTY:
        case CLIP_MUTED:
            break;

        case CLIP_SYNC_LAUNCH:
            PadSetColor(i, CLIP_COL_PLAY );
            KikpadPadNoteOn(i, &pk );
            KikpadClip[i].pos = MPCBar ;
            KikpadClip[i].state = CLIP_PLAYING ;
            KikpadClip[i].countBar = 1 ;
            break;

        case CLIP_SYNC_ONESHOT:
            if ( KikpadClip[i].pos == MPCBar ) {
              PadSetColor(i, CLIP_COL_PLAY );
              KikpadPadNoteOn(i, &pk );
              KikpadClip[i].state = CLIP_PLAYING ;
              KikpadClip[i].countBar = 1 ;
            } else {
              PadSetColor(i, CLIP_COL_PENDING);
            }
            break;

        case CLIP_PLAYING:
            PadSetColor(i, CLIP_COL_PLAY );
            KikpadClip[i].countBar++;
            break;

        case CLIP_SYNC_MUTE:
            PadSetColor(i, CLIP_COL_PENDING);
            break;

        case CLIP_SYNC_REC:
            PadSetColor(i, CLIP_COL_RECORD);
            if ( KikpadClip[i].lenBar++ == 0 ) {
              KikpadClip[i].pos = MPCBar ;
              KikpadPadNoteOn(i,&pk);
            }
            else if ( KikpadClip[i].lenBar == KIKPAD_MAX_BAR_LEN ) {
              // Max length. Stop recording
              PadSetColor(i, CLIP_COL_PENDING);
              KikpadClip[i].state = CLIP_SYNC_LAUNCH ;
            }
            break;
      }
    }
  }

  // Led off for pads muted or playing to make a pulse effect
  else if ( state == SEQ_PAD_PULSE ) {
      for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
            if ( KikpadClip[i].state == CLIP_PLAYING ) {
                    PadSetColor(i, KikpadPadsColors[i]);
            }
      }
  }

  else if ( state == SEQ_LAST_TICK_IN_BAR ) {

    // Last tick/beat before a new bar : we need to send clip note off here
    // to avoid notes overlap at the beggining of a new bar
    // Mute doesn't wait end of clip
    for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {


        // Loop if necessary
        if ( KikpadClip[i].state == CLIP_PLAYING ) {

            if ( KikpadClip[i].countBar == KikpadClip[i].lenBar) {
              if ( KikpadClip[i].loop) {
                KikpadClip[i].state = CLIP_SYNC_LAUNCH;
              }
              else {
                KikpadClip[i].state = CLIP_SYNC_ONESHOT;
              }

              KikpadPadNoteOff(i,&pk);
            }
        }
        else

        // Mute a clip
        if ( KikpadClip[i].state == CLIP_SYNC_MUTE) {
            KikpadClip[i].state = CLIP_MUTED ;
            KikpadPadNoteOff(i,&pk);
            PadSetColor(i, KikpadPadsColors[i]);
        }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Midi clocked actions
///////////////////////////////////////////////////////////////////////////////
static void MPCClockEvent() {

  MPCClock++;

  if (MPCPlaying) {

    // Start a new beat
    if ( ++MPCTick == 1 ) {
      // Start a new bar
      if ( MPCBeat == 0 ) {
        KikpadClipSequencer(SEQ_NEW_BAR);

      }
      ButtonSetLed(MPCButtonsMap[PLAY].btnValue, ON);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB].btnValue, ON);
    }

    // Ticks
    else {
      if ( MPCTick == 6 ) {
        ButtonSetLed(MPCButtonsMap[PLAY].btnValue, OFF);
        if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB].btnValue, OFF);
      }
      else

      if ( MPCTick == 12  && MPCBeat == 3 ) KikpadClipSequencer(SEQ_PAD_PULSE);
      else

      if ( MPCTick == 24 ) {
          // Finish a current bar
          if ( MPCBeat == 3 ) KikpadClipSequencer(SEQ_LAST_TICK_IN_BAR);
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
// Clear a clip Row
///////////////////////////////////////////////////////////////////////////////
static void KikpadClearClipRow(uint8_t row) {

  uint8_t start = row * 8 ;

  for ( uint8_t i = start  ; i < start + 8 ; i++ ) {
    if ( KikpadClip[i].state != CLIP_EMPTY ) {
      KikpadClipClear(i);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Launch a clip Row
///////////////////////////////////////////////////////////////////////////////
static void KikpadLaunchClipRow(uint8_t row,boolean launch) {

  uint8_t start = row * 8 ;

  for ( uint8_t i = start  ; i < start + 8 ; i++ ) {
    if ( KikpadClip[i].state != CLIP_EMPTY ) {
      KikpadClip[i].state = (launch ? CLIP_SYNC_LAUNCH:CLIP_SYNC_MUTE);
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
          KikpadClip[i].state = CLIP_MUTED ;
          KikpadClip[i].countBar = 0 ;
          PadSetColor(i, KikpadPadsColors[i]);
      }
      else if ( KikpadClip[i].state == CLIP_SYNC_LAUNCH ) {
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
    if ( ButtonIsHolded( BT_MS1 + i ) ) {
      PadSetColor(idx, CLIP_COL_PENDING);
      KikpadClip[idx].lenBar = i + 1 ;
      if (KikpadClip[i].countBar > KikpadClip[idx].lenBar) KikpadClip[i].countBar = 0 ;
      KikpadClip[idx].state =  CLIP_SYNC_LAUNCH  ;
      ShowClipLen(KikpadClip[idx].lenBar) ;
      return KikpadClip[idx].lenBar;
    }
  }

  return  0;

}

///////////////////////////////////////////////////////////////////////////////
// INITIALIZE BOTTOM MODES BUTTON BAR LEDS
///////////////////////////////////////////////////////////////////////////////
static void InitModesBarLeds() {
  ButtonSetLed(MPCButtonsMap[PLAY_START].btnValue, ON);
  ButtonSetLed(MPCButtonsMap[PLAY].btnValue, ON);
  ButtonSetLed(MPCButtonsMap[TAP_TEMPO].btnValue, ON);
  ButtonSetLed(MPCButtonsMap[OVERDUB].btnValue, ON);
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
        ButtonSetLed(MPCButtonsMap[STOP].btnValue, ON);
        KikpadClipStartStopAll(true);
        MPCClock = MPCBar = MPCBeat = MPCTick = 0;

        break;

      // Continue
      case 0xFB:
        MPCPlaying = true;
        ButtonSetLed(MPCButtonsMap[STOP].btnValue, ON);

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
// Pad scene mode fro track n
///////////////////////////////////////////////////////////////////////////////
static void MPCPadSceneTrack(uint8_t track) {

  // Go to track
  MPCGotoTrack(track);

  // Move to pad scene mode ()
  MPCButtonNoteOn(FIRST_QLINK_MODE);
  for ( uint8_t i = 0 ; i < 3 ; i++ ) {
    MPCButtonNoteOn(NEXT_QLINK_MODE);
  }

}

///////////////////////////////////////////////////////////////////////////////
// NAVIGATE TO A SPECIFIC TRACK (MACRO). 0 is the first track.
///////////////////////////////////////////////////////////////////////////////
static void MPCGotoTrack(uint8_t track) {


  // Go to track 1
  MPCButtonNoteOn(FIRST_TRACK);

  // Move to track
  for ( uint8_t i = 0 ; i < track ; i++ ) {
    MPCButtonNoteOn(NEXT_TRACK);
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
// SEND A MPC Mapped Button Ctrl ON on cable 0
///////////////////////////////////////////////////////////////////////////////
static void MPCButtonNoteOn(uint8_t btnValue) {

  midiPacket_t pk = { .i = 0 };

  // Note On
  if ( MPCButtonsMap[btnValue].type == MAP_TO_NOTE ) {
    // Note on - cable 0
    pk.packet[0] = 0x09 ;
    pk.packet[1] = 0x90 + MPC_BUTTON_MIDI_CHANNEL ;
  } else
  // CC
  if ( MPCButtonsMap[btnValue].type == MAP_TO_CC ) {
    // Note on - cable 0
    pk.packet[0] = 0x0B ;
    pk.packet[1] = 0xB0 + MPC_BUTTON_MIDI_CHANNEL ;
  }
  else return;

  // Send values
  pk.packet[2] = MPCButtonsMap[btnValue].value1 ;
  pk.packet[3] = MPCButtonsMap[btnValue].value2 ;

  MidiUSB.writePacket(&pk.i);

}

///////////////////////////////////////////////////////////////////////////////
// SEND A MPC Mapped Button Note OFF on cable 0
///////////////////////////////////////////////////////////////////////////////
static void MPCButtonNoteOff(uint8_t btnValue) {

  // Only if mapped to note
  if ( MPCButtonsMap[btnValue].type != MAP_TO_NOTE ) return;

  // Note on - cable 0

  midiPacket_t pk = { .i = 0 };

  // Note off
  pk.packet[0] = 0x08 ;
  pk.packet[1] = 0x80 + MPC_BUTTON_MIDI_CHANNEL ;

  // Send values
  pk.packet[2] = MPCButtonsMap[btnValue].value1 ;
  pk.packet[3] = 0 ;

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
  pk->packet[3] = 0x00 ;
  MidiUSB.writePacket(&pk->i);
}

///////////////////////////////////////////////////////////////////////////////
// PARSE A RECEIVED USER EVENT
///////////////////////////////////////////////////////////////////////////////
void KikpadMod_ProcessUserEvent(UserEvent_t *ev){

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
        // Compute time between 2 encoders events
        static unsigned long lastEncoderEventMicros = micros();
        unsigned long encoderEventDelayMicros = micros() - lastEncoderEventMicros;
        lastEncoderEventMicros = micros();

        uint8_t e = idx + ( KikpadEncodersBank ? 8 : 0 ) ;
        // Compute accelerator factor regarding the turn speed.
        uint8_t accel = 50 * 1000 / encoderEventDelayMicros ;

        #if defined(ENCODERS_RELATIVE)

          if ( ev->ev == EV_EC_CCW )
            KikpadEncodersVal[e] = 0x7f - accel ;
          else
            KikpadEncodersVal[e] = 1 + accel ;

        #else
          // Compute accelerator factor if <  100 millisecs
          KikpadEncodersVal[e] += ( encoderEventDelayMicros < 100*1000 ? 6:1 ) * ( ev->ev == EV_EC_CW ? 1 : -1);

          // as KikpadEncodersVal is unsigned, > 127 means also negative
          if ( KikpadEncodersVal[e] > 127 ) KikpadEncodersVal[e] = ( ev->ev == EV_EC_CW ? 127 : 0 );

        #endif

        MPCEncoderSendCC(e,KikpadEncodersVal[e]);
        break;
      }

    // Pad pressed and not released
    case EV_PAD_PRESSED:
        PadPressed = true ;
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
        PadPressed = false ;
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
    // Button pressed and holded more than 2s
    case EV_BTN_PRESSED:
        ButtonPressed = true;

        if ( idx == BT_SET || idx == BT_SENDB || idx == BT_PAN ) {
          ButtonSetLed(idx,ON);
        }
        else

        // VOLUME PRESSED. Toggle encoders bank.
        // 2 encoders banks 0-1. Light Volume if Bank 1 set.
        if ( idx == BT_VOLUME ) {
          KikpadEncodersBank = ! KikpadEncodersBank ;
          ButtonSetLed(BT_VOLUME, KikpadEncodersBank);
        }
        else

        // Master 1 - 8
        if ( idx >= BT_MS1 && idx <= BT_MS8  ) {

          // Change track SET + MASTER 1-8
          if ( ButtonIsPressed(BT_SET) ) MPCGotoTrack(idx - BT_MS1);

          // LAUNCH ROW : BT_SENDB + MASTER 1-8
          else if ( ButtonIsPressed(BT_SENDB) ) KikpadLaunchClipRow(idx - BT_MS1,true);
          // MUTE ROW : BT_SENDB + MASTER 1-8
          else if ( ButtonIsPressed(BT_PAN) ) KikpadLaunchClipRow(idx - BT_MS1,false);
          // CLEAR ROW : MODE1 + MASTER 1-8
          else if ( ButtonIsPressed(BT_MODE1) ) KikpadClearClipRow(idx - BT_MS1);

        }
        else
        // SET functions
        if ( ButtonIsPressed(BT_SET) ) {

          // Toggle Clip mode. SET + CLIP
          if ( idx == BT_CLIP )  {
            KikpadModeClipNote = ! KikpadModeClipNote;
            ButtonSetLed(BT_CLIP,KikpadModeClipNote ? OFF:ON);
          }
        }
        else

        // MODE1 HOLDED : ERASE
        if ( ButtonIsHolded(BT_MODE1) ) {

          // ERASE + CLIP : Erase all clips
          if ( idx == BT_CLIP ) {
              KikpadClipResetAll();
          }

        }

        // Default mapping for MPC buttons
        else {
          int16_t value = MPCButtonGetMap(idx);
          if ( !PadPressed && value  >= 0  ) {
            MPCButtonNoteOn(value);
            if (value == OVERDUB ) {
              MPCOverdub = !MPCOverdub;
              ButtonSetLed(MPCButtonsMap[OVERDUB].btnValue, ON);
            }
          }
        }
        break;

    case EV_BTN_RELEASED:
        {
          ButtonPressed = false;

          if (    idx == BT_SET
               || idx == BT_MODE1
               || ( idx >= BT_MS1 && idx <= BT_MS8 )
               || idx == BT_SENDB
               || idx == BT_PAN
             ) {

            ButtonSetLed(idx,OFF);

          }


          int16_t value = MPCButtonGetMap(idx);
          if ( !PadPressed && value  >= 0  ) {
            MPCButtonNoteOff(value);
          }
        }
        break;

    case EV_BTN_HOLDED:

      if ( idx == BT_MODE1 || ( idx >= BT_MS1 && idx <= BT_MS8 ) ) {
          ButtonSetLed(idx,ON);
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
