/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_data_converter_helpers.h */

#include "libwinter_test.h"
#include "wntr_array.h"
#include "wntr_data_converter_helpers.h"

TEST_CASE_BEGIN(code_points_to_volts)
    /* Basics */
    munit_assert_double_equal(wntr_code_points_to_volts(128, WNTR_RESOLUTION_8_BIT, 0.0f, 2.0f, false), 1.0f, 2);
    munit_assert_double_equal(wntr_code_points_to_volts(512, WNTR_RESOLUTION_10_BIT, 0.0f, 2.0f, false), 1.0f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(2048, WNTR_RESOLUTION_12_BIT, 0.0f, 2.0f, false), 1.0f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(8192, WNTR_RESOLUTION_14_BIT, 0.0f, 2.0f, false), 1.0f, 4);
    munit_assert_double_equal(wntr_code_points_to_volts(32768, WNTR_RESOLUTION_16_BIT, 0.0f, 2.0f, false), 1.0f, 4);

    /* Inverted */
    munit_assert_double_equal(wntr_code_points_to_volts(64, WNTR_RESOLUTION_8_BIT, 0.0f, 2.0f, true), 1.5f, 2);
    munit_assert_double_equal(wntr_code_points_to_volts(256, WNTR_RESOLUTION_10_BIT, 0.0f, 2.0f, true), 1.5f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(1024, WNTR_RESOLUTION_12_BIT, 0.0f, 2.0f, true), 1.5f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(4096, WNTR_RESOLUTION_14_BIT, 0.0f, 2.0f, true), 1.5f, 4);
    munit_assert_double_equal(wntr_code_points_to_volts(16384, WNTR_RESOLUTION_16_BIT, 0.0f, 2.0f, true), 1.5f, 4);

    /* Positive and negative */
    munit_assert_double_equal(wntr_code_points_to_volts(128, WNTR_RESOLUTION_8_BIT, -1.0f, 1.0f, false), 0.0f, 2);
    munit_assert_double_equal(wntr_code_points_to_volts(512, WNTR_RESOLUTION_10_BIT, -1.0f, 1.0f, false), 0.0f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(2048, WNTR_RESOLUTION_12_BIT, -1.0f, 1.0f, false), 0.0f, 3);
    munit_assert_double_equal(wntr_code_points_to_volts(8192, WNTR_RESOLUTION_14_BIT, -1.0f, 1.0f, false), 0.0f, 4);
    munit_assert_double_equal(wntr_code_points_to_volts(32768, WNTR_RESOLUTION_16_BIT, -1.0f, 1.0f, false), 0.0f, 4);
TEST_CASE_END

TEST_CASE_BEGIN(volts_to_code_points)
    /* Basics */
    munit_assert_uint32(wntr_volts_to_code_points(1.0f, WNTR_RESOLUTION_8_BIT, 0.0f, 2.0f, false), ==, 128);
    munit_assert_uint32(wntr_volts_to_code_points(1.0f, WNTR_RESOLUTION_10_BIT, 0.0f, 2.0f, false), ==, 512);
    munit_assert_uint32(wntr_volts_to_code_points(1.0f, WNTR_RESOLUTION_12_BIT, 0.0f, 2.0f, false), ==, 2048);
    munit_assert_uint32(wntr_volts_to_code_points(1.0f, WNTR_RESOLUTION_14_BIT, 0.0f, 2.0f, false), ==, 8192);
    munit_assert_uint32(wntr_volts_to_code_points(1.0f, WNTR_RESOLUTION_16_BIT, 0.0f, 2.0f, false), ==, 32768);

    /* Inverted */
    munit_assert_uint32(wntr_volts_to_code_points(1.5f, WNTR_RESOLUTION_8_BIT, 0.0f, 2.0f, true), ==, 63);
    munit_assert_uint32(wntr_volts_to_code_points(1.5f, WNTR_RESOLUTION_10_BIT, 0.0f, 2.0f, true), ==, 255);
    munit_assert_uint32(wntr_volts_to_code_points(1.5f, WNTR_RESOLUTION_12_BIT, 0.0f, 2.0f, true), ==, 1023);
    munit_assert_uint32(wntr_volts_to_code_points(1.5f, WNTR_RESOLUTION_14_BIT, 0.0f, 2.0f, true), ==, 4095);
    munit_assert_uint32(wntr_volts_to_code_points(1.5f, WNTR_RESOLUTION_16_BIT, 0.0f, 2.0f, true), ==, 16383);

    /* Postive and negative */
    munit_assert_uint32(wntr_volts_to_code_points(0.0f, WNTR_RESOLUTION_8_BIT, -1.0f, 1.0f, false), ==, 128);
    munit_assert_uint32(wntr_volts_to_code_points(0.0f, WNTR_RESOLUTION_10_BIT, -1.0f, 1.0f, false), ==, 512);
    munit_assert_uint32(wntr_volts_to_code_points(0.0f, WNTR_RESOLUTION_12_BIT, -1.0f, 1.0f, false), ==, 2048);
    munit_assert_uint32(wntr_volts_to_code_points(0.0f, WNTR_RESOLUTION_14_BIT, -1.0f, 1.0f, false), ==, 8192);
    munit_assert_uint32(wntr_volts_to_code_points(0.0f, WNTR_RESOLUTION_16_BIT, -1.0f, 1.0f, false), ==, 32768);
TEST_CASE_END

TEST_CASE_BEGIN(calc_errors)
    float gain_error, offset_error;

    /* Offset error only, no gain error */
    wntr_calculate_gain_and_offset_error(0.1f, 1.1f, 0.0f, 1.0f, &gain_error, &offset_error);
    munit_assert_double_equal(gain_error, 1.0f, 3);
    munit_assert_double_equal(offset_error, 0.1f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.1f, gain_error, offset_error), 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.2f, gain_error, offset_error), 0.1f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.3f, gain_error, offset_error), 0.2f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(1.1f, gain_error, offset_error), 1.0f, 3);

    /* Same, but negative offset error */
    wntr_calculate_gain_and_offset_error(-0.3f, 0.7f, 0.0f, 1.0f, &gain_error, &offset_error);
    munit_assert_double_equal(gain_error, 1.0f, 3);
    munit_assert_double_equal(offset_error, -0.3f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(-0.3f, gain_error, offset_error), 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(-0.2f, gain_error, offset_error), 0.1f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.0f, gain_error, offset_error), 0.3f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.7f, gain_error, offset_error), 1.0f, 3);

    /* No offset error, just gain error */
    wntr_calculate_gain_and_offset_error(0.0f, 1.1f, 0.0f, 1.0f, &gain_error, &offset_error);
    munit_assert_double_equal(gain_error, 1.1f, 3);
    munit_assert_double_equal(offset_error, 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.0f, gain_error, offset_error), 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(1.1f, gain_error, offset_error), 1.0f, 3);

    /* Same, but with gain error < 1.0 */
    wntr_calculate_gain_and_offset_error(0.0f, 0.9f, 0.0f, 1.0f, &gain_error, &offset_error);
    munit_assert_double_equal(gain_error, 0.9f, 3);
    munit_assert_double_equal(offset_error, 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.0f, gain_error, offset_error), 0.0f, 3);
    munit_assert_double_equal(wntr_apply_error_correction(0.9f, gain_error, offset_error), 1.0f, 3);
TEST_CASE_END

static struct WntrVoltageCalibrationTableEntry identity_table_entries[] = {
    {.expected = 0.0f, .measured = 0.0f},
    {.expected = 0.1f, .measured = 0.1f},
    {.expected = 0.2f, .measured = 0.2f},
    {.expected = 0.3f, .measured = 0.3f},
    {.expected = 0.4f, .measured = 0.4f},
    {.expected = 0.5f, .measured = 0.5f},
    {.expected = 0.6f, .measured = 0.6f},
    {.expected = 0.7f, .measured = 0.7f},
    {.expected = 0.8f, .measured = 0.8f},
    {.expected = 0.9f, .measured = 0.9f},
    {.expected = 1.0f, .measured = 1.0f},
};

static struct WntrVoltageCalibrationTable identity_table = {
    .entries = identity_table_entries,
    .len = WNTR_ARRAY_LEN(identity_table_entries),
};

TEST_CASE_BEGIN(voltage_table_identity)
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.0f, identity_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.05f, identity_table), 0.05f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.5f, identity_table), 0.5f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.55f, identity_table), 0.55f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(1.0f, identity_table), 1.0f, 3);

    /* Test limits */
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(-1.0f, identity_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(1.1f, identity_table), 1.0f, 3);
TEST_CASE_END

static struct WntrVoltageCalibrationTableEntry shifted_table_entries[] = {
    {.expected = 0.0f, .measured = 0.1f},
    {.expected = 0.1f, .measured = 0.2f},
    {.expected = 0.2f, .measured = 0.3f},
    {.expected = 0.3f, .measured = 0.4f},
    {.expected = 0.4f, .measured = 0.5f},
    {.expected = 0.5f, .measured = 0.6f},
    {.expected = 0.6f, .measured = 0.7f},
    {.expected = 0.7f, .measured = 0.8f},
    {.expected = 0.8f, .measured = 0.9f},
    {.expected = 0.9f, .measured = 1.0f},
    {.expected = 1.0f, .measured = 1.1f},
};

static struct WntrVoltageCalibrationTable shifted_table = {
    .entries = shifted_table_entries,
    .len = WNTR_ARRAY_LEN(shifted_table_entries),
};

TEST_CASE_BEGIN(voltage_table_shifted)
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.1f, shifted_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.15f, shifted_table), 0.05f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.6f, shifted_table), 0.5f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.65f, shifted_table), 0.55f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(1.1f, shifted_table), 1.0f, 3);

    /* Test limits */
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(-1.0f, shifted_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(1.1f, shifted_table), 1.0f, 3);
TEST_CASE_END

static struct WntrVoltageCalibrationTableEntry weird_table_entries[] = {
    {.expected = 0.0f, .measured = 0.0f},
    {.expected = 0.1f, .measured = 10.0f},
    {.expected = 0.2f, .measured = 40.0f},
};

static struct WntrVoltageCalibrationTable weird_table = {
    .entries = weird_table_entries,
    .len = WNTR_ARRAY_LEN(weird_table_entries),
};

TEST_CASE_BEGIN(voltage_table_weird)
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(0.0f, weird_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(5.0f, weird_table), 0.05f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(25.0f, weird_table), 0.15f, 3);

    /* Test limits */
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(-1.0f, weird_table), 0.0f, 3);
    munit_assert_double_equal(WntrVoltageCalibrationTable_lookup(100.0f, weird_table), 0.2f, 3);
TEST_CASE_END

static MunitTest test_suite_tests[] = {
    {.name = "code points to volts", .test = test_code_points_to_volts},
    {.name = "volts to code points", .test = test_volts_to_code_points},
    {.name = "calculate errors", .test = test_calc_errors},
    {.name = "voltage table w/ identity", .test = test_voltage_table_identity},
    {.name = "voltage table w/ shifted", .test = test_voltage_table_shifted},
    {.name = "voltage table w/ weird", .test = test_voltage_table_weird},
    {.test = NULL},
};

MunitSuite test_data_conv_suite = {
    .prefix = "data conv: ",
    .tests = test_suite_tests,
    .iterations = 1,
};
