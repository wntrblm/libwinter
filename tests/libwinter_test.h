/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

#pragma once

#include "munit.h"

/* Common test macros and such. */

#pragma GCC diagnostic ignored "-Wdouble-promotion"

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))

#define TEST_CASE_BEGIN(name)                                                                                          \
    static MunitResult test_##name(const MunitParameter params[], void* data) {                                        \
        (void)params;                                                                                                  \
        (void)data;

#define TEST_CASE_END                                                                                                  \
    return MUNIT_OK;                                                                                                   \
    }

/* Common includes */

#include "fix16.h"
#include "munit.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Helper methods */

inline static void print_hex(const uint8_t* buf, size_t len) {
    fprintf(stderr, "[");
    for (size_t i = 0; i < len; i++) {
        if (i > 0)
            fprintf(stderr, ", ");
        fprintf(stderr, "0x%02X", buf[i]);
    }
    fprintf(stderr, "]\n");
}

inline static void print_f16(const fix16_t val) {
    char result[16];
    fix16_to_str(val, result, 4);
    printf("%s", result);
}

#define ASSERT_FIX16_CLOSE(val, target, epsilon)                                                                       \
    {                                                                                                                  \
        fix16_t abs_ = fix16_abs(fix16_sub(val, target));                                                              \
        fix16_t eps_ = F16(epsilon);                                                                                   \
        munit_assert_int32(abs_, <=, eps_);                                                                            \
    }

#define ASSERT_FIX16_GT(val, target) munit_assert_int32(val, >, F16(target));
#define ASSERT_FIX16_LT(val, target) munit_assert_int32(val, <, F16(target));

#undef munit_assert_double_equal
#define munit_assert_double_equal(a, b, precision)                                                                     \
    do {                                                                                                               \
        const double munit_tmp_a_ = (a);                                                                               \
        const double munit_tmp_b_ = (b);                                                                               \
        const double munit_tmp_diff_ =                                                                                 \
            ((munit_tmp_a_ - munit_tmp_b_) < 0) ? -(munit_tmp_a_ - munit_tmp_b_) : (munit_tmp_a_ - munit_tmp_b_);      \
        if (isnan(munit_tmp_diff_)) {                                                                                  \
            munit_errorf("assertion failed: %s == %s, found NaN", #a, #b);                                             \
        }                                                                                                              \
        if (isinf(munit_tmp_diff_)) {                                                                                  \
            munit_errorf("assertion failed: %s == %s, found Inf", #a, #b);                                             \
        }                                                                                                              \
        if (MUNIT_UNLIKELY(munit_tmp_diff_ > 1e-##precision)) {                                                        \
            munit_errorf(                                                                                              \
                "assertion failed: %s == %s (%0." #precision "g == %0." #precision "g)",                               \
                #a,                                                                                                    \
                #b,                                                                                                    \
                munit_tmp_a_,                                                                                          \
                munit_tmp_b_);                                                                                         \
        }                                                                                                              \
    } while (0);

/* Suites */

MunitSuite test_midi_core_suite;
MunitSuite test_midi_sysex_suite;
MunitSuite test_bezier_suite;
MunitSuite test_bitbang_spi_suite;
MunitSuite test_data_conv_suite;
MunitSuite test_smoothie_float_suite;
MunitSuite test_smoothie_fix16_suite;
MunitSuite test_teeth_suite;
