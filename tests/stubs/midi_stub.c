#include "midi_stub.h"

uint8_t usb_midi_in_packets[1024];
size_t usb_midi_in_packets_idx = 0;
uint8_t usb_midi_out_packets[1024];
size_t usb_midi_out_packets_idx = 0;

bool tud_midi_n_receive(uint8_t idx, uint8_t packet[4]) {
    (void)(idx);

    if (usb_midi_in_packets[usb_midi_in_packets_idx] == 0) {
        return false;
    }
    packet[0] = usb_midi_in_packets[usb_midi_in_packets_idx];
    packet[1] = usb_midi_in_packets[usb_midi_in_packets_idx + 1];
    packet[2] = usb_midi_in_packets[usb_midi_in_packets_idx + 2];
    packet[3] = usb_midi_in_packets[usb_midi_in_packets_idx + 3];
    usb_midi_in_packets_idx += 4;
    fprintf(stderr, "Yielded packet 0x%02x 0x%02x 0x%02x 0x%02x\n", packet[0], packet[1], packet[2], packet[3]);
    return true;
}

bool tud_midi_n_send(uint8_t idx, const uint8_t packet[4]) {
    (void)(idx);
    usb_midi_out_packets[usb_midi_out_packets_idx] = packet[0];
    usb_midi_out_packets[usb_midi_out_packets_idx + 1] = packet[1];
    usb_midi_out_packets[usb_midi_out_packets_idx + 2] = packet[2];
    usb_midi_out_packets[usb_midi_out_packets_idx + 3] = packet[3];
    usb_midi_out_packets_idx += 4;
    return true;
}

void reset_usb_midi() {
    usb_midi_out_packets_idx = 0;
    memset(usb_midi_out_packets, 0, ARRAY_LEN(usb_midi_out_packets));
    usb_midi_in_packets_idx = 0;
    memset(usb_midi_in_packets, 0, ARRAY_LEN(usb_midi_in_packets));
}
