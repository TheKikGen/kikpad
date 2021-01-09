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

// KIKPAD_DEMO : A Midi out only basic keyboard

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
  // 32 pads defined as midi keyboard. Offset + 32
  // -1 = No Note
  // -2 = transpose - 12
  // -3 = transpose + 12
  static const int8_t keyBoardMap[] {
    //    C#4  D#4       F#4  G#4  A#4
    -1,  0x3D,0x3F, -1 ,0x42,0x44,0x46, -1,
    // C4  D4  E4   F4   G4   A4   B4   C5
    0x3C,0x3E,0x40,0x41,0x43,0x45,0x47,0x48,
    //    C#3  D#3       F#3  G#3  A#3
    -2,  0x31,0x33, -1 ,0x36,0x38,0x3A, -3,
    // C3  D3  E3   F3   G3   A3   B3   C4
    0x30,0x32,0x34,0x35,0x37,0x39,0x3B,0x3C
  };
  static const int8_t keyBoardPadColors[]{
    BLACK, BLUE,  BLUE,  BLACK, BLUE,  BLUE,  BLUE,  BLACK,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    GREEN, BLUE,  BLUE,  BLACK, BLUE,  BLUE,  BLUE,  GREEN,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
  };

  static boolean setupMode = true;
  static uint8_t currentSendMode=0;
  static const uint8_t encoderCC[8]={CC_VOL,CC_EFFECT1,CC_EFFECT2,CC_PAN,CC_CUTOFF,CC_RESO,0,0};
  static uint8_t encoderVal[8] = {0,0,0,0,0,0,0,0};
  static uint8_t midiChTranspose = 0;
  static uint8_t midiChannel = 0;
  static int8_t midiNoteTranspose = 0;

  midiPacket_t pk2;

  if (setupMode ) {
      ButtonsLedStates[0] = BTMSK_VOLUME ;
      ButtonsLedStates[1] = BTMSK_MS1 ;
      PadColorsBackground(BLACK);
      // Copy Keyboard pad colors on the lower 32 pads
      memcpy(PadColorsCurrent+32,keyBoardPadColors,32);
      RGBMaskUpdateAll();

      PadLedStates[0] = PadLedStates[1] = 0XFFFFFFFF ;
      currentSendMode = 0;
      midiChannel = 0;
      setupMode = false;
  }

  uint8_t idx = SCAN_IDX(ev->d1,ev->d2);

  switch (ev->ev) {

    // Encoders Clock wise
    case EV_EC_CW:
      if ( ++encoderVal[idx] > 127 ) encoderVal[idx] = 127;
      pk2.packet[0] = 0x0B;
      pk2.packet[1] = 0xB0 + idx + midiChTranspose;
      pk2.packet[2] = encoderCC[currentSendMode];
      pk2.packet[3] = encoderVal[idx];
      MidiUSB.writePacket(&pk2.i);

      break;

    // Encoders Counter Clock wise
    case EV_EC_CCW:
      if ( encoderVal[idx] > 0 ) encoderVal[idx]--;
      pk2.packet[0] = 0x0B;
      pk2.packet[1] = 0xB0 + idx + midiChTranspose;
      pk2.packet[2] = encoderCC[currentSendMode];
      pk2.packet[3] = encoderVal[idx];
      MidiUSB.writePacket(&pk2.i);


      break;

    // Pad pressed and not released
    case EV_PAD_PRESSED:
      // Keyboard  : pad 32 to 63
      if ( idx >= 32 ) {
        uint8_t i = idx-32;
        if ( keyBoardMap[i] >= 0 ) {
          PadColorsBackup[idx] = PadColorsCurrent[idx];
          PadSetColor(idx, RED);
          // Send Note On
          pk2.packet[0] = 0x09;
          pk2.packet[1] = 0x90 + midiChannel + midiChTranspose;
          pk2.packet[2] = keyBoardMap[i]  + midiNoteTranspose;
          pk2.packet[3] = 0x7F;
          MidiUSB.writePacket(&pk2.i);
        }
        else if (keyBoardMap[i] == -2 ) {
          midiNoteTranspose -= 12;
          if ( midiNoteTranspose > 0) {
            PadSetColor(48, GREEN);
            PadSetColor(55, CYAN);
          }
          else if ( midiNoteTranspose == 0) {
            PadSetColor(48, GREEN);
            PadSetColor(55, GREEN);
          }
          else {
            PadSetColor(48, CYAN);
            PadSetColor(55, GREEN);
          }
        }
      }

      break;

    // Pad released
    case EV_PAD_RELEASED:
      // Keyboard  : pad 32 to 63
      if ( idx >= 32 ) {
        uint8_t i = idx-32;
        if ( keyBoardMap[i] >= 0 ) {
          PadSetColor(idx, PadColorsBackup[idx]);
          // Send Note Off
          pk2.packet[0] = 0x08;
          pk2.packet[1] = 0x80 + midiChannel + midiChTranspose;
          pk2.packet[2] = keyBoardMap[i]  + midiNoteTranspose;
          pk2.packet[3] = 0x40;
          MidiUSB.writePacket(&pk2.i);
        }
        else if (keyBoardMap[i] == -2 ) {
          midiNoteTranspose += 12;
          if ( midiNoteTranspose > 0) {
            PadSetColor(48, GREEN);
            PadSetColor(55, CYAN);
          }
          else if ( midiNoteTranspose == 0) {
            PadSetColor(48, GREEN);
            PadSetColor(55, GREEN);
          }
          else {
            PadSetColor(48, CYAN);
            PadSetColor(55, GREEN);
          }
        }
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

}
#endif
