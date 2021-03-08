/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

#include <stdbool.h>
#include <stdint.h>

bool tud_midi_n_receive(uint8_t idx, uint8_t packet[4]);
bool tud_midi_n_send(uint8_t idx, const uint8_t packet[4]);

static inline bool tud_midi_receive(uint8_t packet[4]) { return tud_midi_n_receive(0, packet); }

static inline bool tud_midi_send(uint8_t const packet[4]) { return tud_midi_n_send(0, packet); }
