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
// CLIP                        = TOGGLE CLIP MODE. MIDI NOTE ON/OFF ONLY WHEN OFF
// VOLUME HOLDED + ENCODER     = UPPER ENCODER BANK 8-16 (SHIFT)
// SET + VOLUME PRESSED        = LOCK ENCODER BANK 8-16
// MASTER 1-4 + PAD PRESSED    = CHANGE CLIP LENGTH 1 - 2 - 3 - 4
// MODE1 HOLDED + PAD PRESSED  = CLEAR CLIP
// MODE1 HOLDED + MOD2 HOLDED  = CLEAR ALL CLIPS
////////////////////////////////////////////////////////////////////////////////

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
static void InitModesBarLeds();
static void KikpadClipsNoteOff();

// Globals

static boolean  MPCPlaying = false;
static boolean  MPCOverdub = false;
static uint16_t MPCBar   = 0;
static uint8_t  MPCBeat  = 0;
static uint16_t MPCClock = 0;
static uint8_t  MPCTick  = 0;

// True if clip mode active
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
  uint8_t countBar;
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
        if ( KikpadModeClip ) KikpadClipSeqStateMachine();
        ShowCurrentMPCBar();
      }
      ButtonSetLed(MPCButtonsMap[PLAY], ON);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], ON);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], ON);

    }
    else if ( MPCTick == 6 ) {
      ButtonSetLed(MPCButtonsMap[PLAY], OFF);
      ButtonSetLed(MPCButtonsMap[TAP_TEMPO], OFF);
      if (MPCOverdub) ButtonSetLed(MPCButtonsMap[OVERDUB], OFF);
    }
    else if ( MPCTick == 24 ) {
        // Finish a current bar
        if ( KikpadModeClip && MPCBeat == 3 ) KikpadClipsNoteOff();
        MPCTick = 0 ;
        if ( ++MPCBeat == 4 ) {        // Bar
            MPCBeat = 0 ;
            if ( ++MPCBar == KikpadClipMaxBarLen ) MPCBar = 0;
        }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Clear a Kikpad clip
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipClear(uint8_t idx) {

  KikpadClip[idx].state = CLIP_EMPTY ;
  KikpadClip[idx].lenBar = 0;
  KikpadClip[idx].countBar = 0 ;
  if ( KikpadClipMaxIdx && idx == KikpadClipMaxIdx ) KikpadClipMaxIdx-- ;
  if ( KikpadClipMinIdx != KikpadClipMaxIdx && idx == KikpadClipMinIdx ) KikpadClipMinIdx++ ;
  KikpadPadSendNote(idx,false); // Note OFF
  PadSetColor(idx,KikpadPadsColors[idx]);

}

///////////////////////////////////////////////////////////////////////////////
// Kikpad reset all clips
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipResetAll() {

  for ( uint8_t i ; i < PAD_SIZE ; i++ ) {
    KikpadClip[i].state = CLIP_EMPTY ;
    KikpadClip[i].lenBar = 0;
    KikpadClip[i].countBar = 0 ;
    KikpadPadSendNote(i,false); // Note OFF
  }
  KikpadClipMaxIdx = 0;
  KikpadClipMinIdx = PAD_SIZE ;
  memcpy(PadColorsCurrent,KikpadPadsColors,sizeof(KikpadPadsColors));
  RGBMaskUpdateAll();

}

///////////////////////////////////////////////////////////////////////////////
// Kikpad stop all clips but keep clip length
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipStopAll() {

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
    if ( KikpadClip[i].state == CLIP_SYNC_PLAY ) {
      KikpadPadSendNote(i,false); // Note OFF
      //KikpadClip[i].state = CLIP_STOPPED;
      PadSetColor(i,CLIP_COL_PENDING);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Change Len of a clip. Return len if done.
///////////////////////////////////////////////////////////////////////////////
static uint8_t KikpadClipChangeLen(uint8_t idx) {

  // Change length if MS-1 MS-4 pressed
  // That will reset position at the next bar
  for ( uint8_t i = 0 ; i < KikpadClipMaxBarLen ; i++ ) {
    if ( ButtonIsPressed( BT_MS1 + i ) ) {
      PadSetColor(idx, CLIP_COL_PENDING);
      KikpadClip[idx].lenBar = i + 1 ;
      KikpadClip[i].countBar = 0 ;
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

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {
        if ( KikpadClip[i].state == CLIP_SYNC_MUTE
              || ( !KikpadClip[i].countBar && KikpadClip[i].state == CLIP_SYNC_PLAY) ) {

                KikpadPadSendNote(i,false); // Note OFF
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Launch and loop clip - Clip sequencer state machine
///////////////////////////////////////////////////////////////////////////////
static void KikpadClipSeqStateMachine() {

  for ( int8_t i = KikpadClipMaxIdx ; i >= KikpadClipMinIdx ; i --) {

    switch (KikpadClip[i].state) {

      case CLIP_EMPTY:
          break;

      case CLIP_MUTED:
          break;

      case CLIP_SYNC_PLAY:
          if ( KikpadClip[i].countBar++ == 0 )  {
            PadSetColor(i, CLIP_COL_PLAY );
            KikpadPadSendNote(i,true,KikpadPadVelocity );
            //KikpadClip[i].countBar = KikpadClip[i].lenBar - 1;
          }
          if ( KikpadClip[i].countBar == KikpadClip[i].lenBar ) {
            KikpadClip[i].countBar = 0;
          }
          break;

      case CLIP_SYNC_MUTE:
          KikpadClip[i].countBar = 0 ;
          KikpadClip[i].state = CLIP_MUTED ;
          PadSetColor(i, KikpadPadsColors[i]);
          break;

      case CLIP_SYNC_REC:
          if ( KikpadClip[i].lenBar++ == 0 ) {
            KikpadPadSendNote(i,true,KikpadPadVelocity);
            PadSetColor(i, CLIP_COL_RECORD);
          }
          else if ( KikpadClip[i].lenBar == KikpadClipMaxBarLen ) {
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

    if ( KikpadClip[idx].state != CLIP_EMPTY ) {
      // Specific clip actions

      // Change length eventually if MS-1 MS-4 pressed
      // That will reset position at the next bar
      if ( KikpadClipChangeLen(idx) ) return ;

      // Clear a clip if MODE1 Holded
      // Will send a note off immediatly
      if ( ButtonIsHolded(BT_MODE1) ) {
          KikpadClipClear(idx);
          return;
      }

      // Start a clip if stopped
      if ( KikpadClip[idx].state == CLIP_MUTED ) {
        PadSetColor(idx, CLIP_COL_PENDING);
        KikpadClip[idx].state = CLIP_SYNC_PLAY;
      }
      else

      // Stop a clip if playing
      if ( KikpadClip[idx].state ==  CLIP_SYNC_PLAY ) {
        PadSetColor(idx, CLIP_COL_PENDING);
        KikpadClip[idx].state = CLIP_SYNC_MUTE;
      }


    }
    // Clip empty
    else {

      // Record clip
      PadSetColor(idx, CLIP_COL_PENDING);

      // seq Loop lazy optimization
      if ( idx > KikpadClipMaxIdx ) KikpadClipMaxIdx = idx ;
      if ( idx < KikpadClipMinIdx)  KikpadClipMinIdx = idx ;

      KikpadClip[idx].state = CLIP_SYNC_REC ;

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
  ButtonSetLed(BT_CLIP,KikpadModeClip ? ON:OFF);
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
        MPCClock = MPCBar = MPCBeat = MPCTick = 0;
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
        InitModesBarLeds();
        KikpadClipStopAll();

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
  pk.packet[2] = value ;
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

  pk.packet[2] = KikpadPadsNotesMap[idx] ;
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

          // Clear all clips in clip mode - HOLD MODE1 THEN HOLD MODE 2
          if ( KikpadModeClip && idx == BT_MODE2 && ButtonIsHolded(BT_MODE1) ) {
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
