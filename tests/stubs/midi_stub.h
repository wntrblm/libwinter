#pragma once

#include "libwinter_test.h"
#include <stddef.h>
#include <stdint.h>

extern uint8_t usb_midi_in_packets[1024];
extern size_t usb_midi_in_packets_idx;
extern uint8_t usb_midi_out_packets[1024];
extern size_t usb_midi_out_packets_idx;

void reset_usb_midi();
