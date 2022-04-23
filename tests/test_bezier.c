/*
    Copyright (c) 2021 Alethea Katherine Flowers.
    Published under the standard MIT License.
    Full text available at: https://opensource.org/licenses/MIT
*/

/* Tests for src/wntr_bezier.c */

#include "fix16.h"
#include "libwinter_test.h"
#include "wntr_bezier.h"

TEST_CASE_BEGIN(1d_mostly_linear)
    fix16_t result = wntr_bezier_cubic_1d(F16(0), F16(0.4), F16(0.6), F16(1.0), F16(0.5));
    ASSERT_FIX16_CLOSE(result, F16(0.5), 0.001);

    for (fix16_t i = F16(0); i < F16(1.0); i = fix16_add(i, F16(0.1))) {
        result = wntr_bezier_cubic_1d(F16(0), F16(0.4), F16(0.6), F16(1.0), i);
        ASSERT_FIX16_CLOSE(result, i, 0.02);
    }
TEST_CASE_END

TEST_CASE_BEGIN(1d_mostly_curvy)
    fix16_t result = wntr_bezier_cubic_1d(F16(0), F16(0.7), F16(0.3), F16(1.0), F16(0.5));
    ASSERT_FIX16_CLOSE(result, F16(0.5), 0.001);

    result = wntr_bezier_cubic_1d(F16(0), F16(0.7), F16(0.3), F16(1.0), F16(0.25));
    ASSERT_FIX16_GT(result, 0.3);

    result = wntr_bezier_cubic_1d(F16(0), F16(0.7), F16(0.3), F16(1.0), F16(0.75));
    ASSERT_FIX16_LT(result, 0.7);
TEST_CASE_END

TEST_CASE_BEGIN(1d_non_normalized)
    fix16_t result = wntr_bezier_cubic_1d(F16(-1.0), F16(-0.3), F16(0.3), F16(1.0), F16(0.5));
    ASSERT_FIX16_CLOSE(result, F16(0), 0.001);

    result = wntr_bezier_cubic_1d(F16(-1.0), F16(-0.3), F16(0.3), F16(1.0), F16(0.25));
    ASSERT_FIX16_GT(result, -0.5);

    result = wntr_bezier_cubic_1d(F16(-1.0), F16(-0.3), F16(0.3), F16(1.0), F16(0.75));
    ASSERT_FIX16_LT(result, 0.5);
TEST_CASE_END

TEST_SUITE_BEGIN
    TEST_SUITE_TEST(1d_mostly_linear, "1d, mostly linear")
    TEST_SUITE_TEST(1d_mostly_curvy, "1d, curvy")
    TEST_SUITE_TEST(1d_non_normalized, "1d, non-normalized")
TEST_SUITE_END(bezier)
