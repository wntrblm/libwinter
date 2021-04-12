/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_midi_sysex_dispatcher.c */

#include "libwinter_test.h"
#include "midi_stub.h"
#include "wntr_midi_core.h"
#include "wntr_midi_sysex_dispatcher.h"

static const uint8_t* captured_sysex_data_;
static size_t captured_sysex_len_;

static void sysex_command_callback(const uint8_t* data, size_t len) {
    captured_sysex_data_ = data;
    captured_sysex_len_ = len;
}

TEST_CASE_BEGIN(basic_dispatch)
    wntr_midi_register_sysex_command(0x3, sysex_command_callback);

    // clang-format off
    const uint8_t midi_packets[] = {
        0x4, 0xF0, WNTR_MIDI_SYSEX_IDENTIFIER, 0x3,
        0x4, 1, 2, 3,
        0x4, 4, 5, 6,
        0x6, 7, 0xF7, 0
    };
    // clang-format on

    memcpy(usb_midi_in_packets, midi_packets, ARRAY_LEN(midi_packets));

    struct WntrMIDIMessage msg = {};
    wntr_midi_receive(&msg);

    munit_assert_uint8(msg.code_index, ==, MIDI_CODE_INDEX_SYSEX_START_OR_CONTINUE);

    wntr_midi_dispatch_sysex();

    /* Command callback should be invoked with just the data payload. */
    const uint8_t expected[] = {1, 2, 3, 4, 5, 6, 7};

    munit_assert_size(captured_sysex_len_, ==, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), captured_sysex_data_, expected);
TEST_CASE_END

static void* setup_midi_tests(const MunitParameter params[], void* user_data) {
    (void)(params);
    (void)(user_data);
    reset_usb_midi();
    return NULL;
}

static MunitTest test_suite_tests[] = {
    {.name = "test basic dispatch", .test = test_basic_dispatch, .setup = setup_midi_tests},
    {.test = NULL},
};

MunitSuite test_midi_sysex_suite = {
    .prefix = "midi sysex: ",
    .tests = test_suite_tests,
    .iterations = 1,
};
