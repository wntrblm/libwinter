/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_midi_core.c */

#include "libwinter_test.h"
#include "midi_stub.h"
#include "wntr_midi.h"

TEST_CASE_BEGIN(send_sysex_one)
    /* one byte -> single packet */
    wntr_midi_send_sysex((const uint8_t[]){1}, 1);

    const uint8_t expected[] = {0x7, 0xF0, 1, 0xF7};
    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_CASE_BEGIN(send_sysex_two)
    /* two bytes -> two packets */
    wntr_midi_send_sysex((const uint8_t[]){1, 2}, 2);

    // clang-format off
    const uint8_t expected[] = {
        0x4, 0xF0, 1, 2,
        0x5, 0xF7, 0, 0
    };
    // clang-format on

    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_CASE_BEGIN(send_sysex_three)
    /* three bytes -> two packets */
    wntr_midi_send_sysex((const uint8_t[]){1, 2, 3}, 3);

    // clang-format off
    const uint8_t expected[] = {
        0x4, 0xF0, 1, 2,
        0x6, 3, 0xF7, 0,
    };
    // clang-format on

    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_CASE_BEGIN(send_sysex_four)
    /* four bytes -> two packets */
    wntr_midi_send_sysex((const uint8_t[]){1, 2, 3, 4}, 4);

    // clang-format off
    const uint8_t expected[] = {
        0x4, 0xF0, 1, 2,
        0x7, 3, 4, 0xF7,
    };
    // clang-format on

    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_CASE_BEGIN(send_sysex_many)
    /* many bytes -> many packets */
    wntr_midi_send_sysex((const uint8_t[]){1, 2, 3, 4, 5, 6, 7, 8, 9}, 9);

    // clang-format off
    const uint8_t expected[] = {
        0x4, 0xF0, 1, 2,
        0x4, 3, 4, 5,
        0x4, 6, 7, 8,
        0x6, 9, 0xF7, 0
    };
    // clang-format on

    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_CASE_BEGIN(receive_sysex_simple)
    // clang-format off
    const uint8_t midi_packets[] = {
        0x4, 0xF0, 1, 2,
        0x4, 3, 4, 5,
        0x4, 6, 7, 8,
        0x6, 9, 0xF7, 0
    };
    // clang-format on

    memcpy(usb_midi_in_packets, midi_packets, ARRAY_LEN(midi_packets));

    struct WntrMIDIMessage msg = {};
    wntr_midi_receive(&msg);

    munit_assert_uint8(msg.code_index, ==, MIDI_CODE_INDEX_SYSEX_START_OR_CONTINUE);

    /* The MIDI sysex data should just be the payload, no header/trailer. */
    const uint8_t expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    munit_assert_size(wntr_midi_sysex_len(), ==, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), wntr_midi_sysex_data(), expected);
TEST_CASE_END

static void* setup_midi_tests(const MunitParameter params[], void* user_data) {
    (void)(params);
    (void)(user_data);
    reset_usb_midi();
    return NULL;
}

TEST_CASE_BEGIN(receive_channel_messages)
    // clang-format off
    const uint8_t midi_packets[] = {
        0x9, 0x91, 64, 127,
        0x8, 0x81, 64, 0,
    };
    // clang-format on

    memcpy(usb_midi_in_packets, midi_packets, ARRAY_LEN(midi_packets));

    struct WntrMIDIMessage msg = {};
    wntr_midi_receive(&msg);

    munit_assert_uint8(msg.code_index, ==, MIDI_CODE_INDEX_NOTE_ON);
    munit_assert_uint8(wntr_midi_get_type(&msg), ==, MIDI_CHANNEL_NOTE_ON);
    munit_assert_uint8(wntr_midi_get_channel(&msg), ==, 1);
    munit_assert_uint8(msg.data_0, ==, 64);
    munit_assert_uint8(msg.data_1, ==, 127);

    wntr_midi_receive(&msg);

    munit_assert_uint8(msg.code_index, ==, MIDI_CODE_INDEX_NOTE_OFF);
    munit_assert_uint8(wntr_midi_get_type(&msg), ==, MIDI_CHANNEL_NOTE_OFF);
    munit_assert_uint8(wntr_midi_get_channel(&msg), ==, 1);
    munit_assert_uint8(msg.data_0, ==, 64);
    munit_assert_uint8(msg.data_1, ==, 0);
TEST_CASE_END

TEST_CASE_BEGIN(send_channel_messages)
    struct WntrMIDIMessage msg = {
        .cable = 0, .code_index = MIDI_CODE_INDEX_NOTE_ON, .status = MIDI_CHANNEL_NOTE_ON, .data_0 = 64, .data_1 = 127};

    wntr_midi_send(&msg);

    const uint8_t expected[] = {0x9, 0x90, 64, 127};
    print_hex(usb_midi_out_packets, ARRAY_LEN(expected));
    munit_assert_memory_equal(ARRAY_LEN(expected), usb_midi_out_packets, expected);
TEST_CASE_END

TEST_SUITE_BEGIN
    TEST_SUITE_TEST(send_sysex_one, "send one byte sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(send_sysex_two, "send two byte sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(send_sysex_three, "send three byte sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(send_sysex_four, "send four byte sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(send_sysex_many, "send multibyte sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(receive_sysex_simple, "receive simple sysex", .setup = setup_midi_tests)
    TEST_SUITE_TEST(receive_channel_messages, "receive channel messages", .setup = setup_midi_tests)
    TEST_SUITE_TEST(send_channel_messages, "send channel messages", .setup = setup_midi_tests)
TEST_SUITE_END(midi_core)
