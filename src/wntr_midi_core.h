/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

#pragma once

/*
   Core MIDI system with handlers and callbacks for incoming MIDI messages.

   Presently this only really reacts to SysEx commands, since that's the
   only thing Gemini uses MIDI for.
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum USBMIDICodeIndexes {
    MIDI_CODE_INDEX_RESERVED_0x0 = 0x0,
    MIDI_CODE_INDEX_RESERVED_0x1 = 0x1,
    MIDI_CODE_INDEX_TWO_BYTE_COMMON = 0x2,
    MIDI_CODE_INDEX_THREE_BYTE_COMMON = 0x3,
    MIDI_CODE_INDEX_SYSEX_START_OR_CONTINUE = 0x4,
    MIDI_CODE_INDEX_SYSEX_END_ONE_BYTE = 0x5,
    MIDI_CODE_INDEX_SYSEX_END_TWO_BYTE = 0x6,
    MIDI_CODE_INDEX_SYSEX_END_THREE_BYTE = 0x7,
    MIDI_CODE_INDEX_NOTE_OFF = 0x8,
    MIDI_CODE_INDEX_NOTE_ON = 0x9,
    MIDI_CODE_INDEX_POLY_KEY_PRESS = 0xA,
    MIDI_CODE_INDEX_CONTROL_CHANGE = 0xB,
    MIDI_CODE_INDEX_PROGRAM_CHANGE = 0xC,
    MIDI_CODE_INDEX_CHANNEL_PRESSURE = 0xD,
    MIDI_CODE_INDEX_PITCH_BEND = 0xE,
    MIDI_CODE_INDEX_UNPARSED_SINGLE_BYTE = 0xF
};

struct WntrMIDIMessage {
    uint8_t cable : 4;
    uint8_t code_index : 4;
    uint8_t midi_0;
    uint8_t midi_1;
    uint8_t midi_2;
};

/* TODO */
bool wntr_midi_task(struct WntrMIDIMessage* msg);
size_t wntr_midi_sysex_len();
const uint8_t* wntr_midi_sysex_data();
void wntr_midi_send_sysex(const uint8_t* data, size_t len);
