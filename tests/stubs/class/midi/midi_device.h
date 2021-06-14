/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

#include <stdbool.h>
#include <stdint.h>

bool tud_midi_packet_read(uint8_t packet[4]);
bool tud_midi_packet_write(uint8_t const packet[4]);
